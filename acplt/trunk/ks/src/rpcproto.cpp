/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/rpcproto.cpp,v 1.2 1998-06-30 11:29:08 harald Exp $ */
/*
 * Copyright (c) 1998
 * Chair of Process Control Engineering,
 * Aachen University of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * rpcproto.cpp -- Implements a class for handling RPC request and reply
 *                 header.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#include "ks/rpcproto.h"


// ---------------------------------------------------------------------------
// Factory method to create a new RPC header object from a XDR stream. It
// allocates and constructs a fresh header object and then tries to deseria-
// lize the header from the XDR stream. If it can't allocate the object or
// deserialization failes, then the object is destroyed and a null pointer is
// returned.
//
KsRpcHeader *KsRpcHeader::xdrNew(XDR *xdrs)
{
    KsRpcHeader *p = new KsRpcHeader;
    if ( p ) {
        if ( !p->xdrDecode(xdrs) ) {
	    delete p;
	    p = 0;
	}
    }
    return p;
} // KsRpcHeader::xdrNew


// ---------------------------------------------------------------------------
// Constructs a fresh new RPC header or constructs it from a XDR stream...
//
KsRpcHeader::KsRpcHeader()
    : _xid(0), _msg_type(KsRpcCALL),
      _rpc_version(2),
      _prog_number(0), _prog_version(0), _procedure(0),
      _reply_status(KsRpcMSGACCEPTED),
      _accept_status(KsRpcSYSTEMERR),
      _low(0), _high(0),
      _auth_error(KsRpcAUTHNONE),
      _reject_status(KsRpcRPCMISMATCH)
{
} // KsRpcHeader::KsRpcHeader

KsRpcHeader::KsRpcHeader(XDR *xdrs, bool &ok)
{
    ok = xdrDecode(xdrs);
} // KsRpcHeader::KsRpcHeader


// ---------------------------------------------------------------------------
// Read in (decode) a RPC header from a XDR stream.
//
bool KsRpcHeader::xdrDecode(XDR *xdrs)
{
    u_long dummy, dummy2;

    _rpc_version = 0; // make sure its invalid now...    
    if ( !xdr_u_long(xdrs, &_xid) ||
    	 !xdr_u_long(xdrs, &dummy) ) {
    	return false;
    }
    _msg_type = (KsRpcMsgType) dummy;
    switch ( _msg_type ) {
    //
    // It's an incomming request (call). So we're usually on the server
    // side when we need do deserialize a call. We don't allow authentification
    // here, so the authentification flavour must be "none" and there must
    // not be any opaque data following it!
    //
    case KsRpcCALL:
    	if ( !xdr_u_long(xdrs, &_rpc_version)  ||
	     !xdr_u_long(xdrs, &_prog_number)  ||
	     !xdr_u_long(xdrs, &_prog_version) ||
	     !xdr_u_long(xdrs, &_procedure)    ||
	     (_rpc_version != 2) ) {
	    return false;
	}
    	if ( !xdr_u_long(xdrs, &dummy)  ||
	     !xdr_u_long(xdrs, &dummy2) ||
	     (dummy != KsRpcAUTHNONE)   || dummy2 ) {
	    return false;
	}
    	if ( !xdr_u_long(xdrs, &dummy)  ||
	     !xdr_u_long(xdrs, &dummy2) ||
	     (dummy != KsRpcAUTHNONE)   || dummy2 ) {
	    return false;
	}
    	return true;
    //
    // It's an incomming reply, so we're on the client side when deserializing
    // a reply.
    //
    case KsRpcREPLY:
    	if ( !xdr_u_long(xdrs, &dummy) ) {
	    return false;
	}
	_reply_status = (KsRpcReplyStat) dummy;
    	switch ( _reply_status ) {
	case KsRpcMSGACCEPTED:
    	    if ( !xdr_u_long(xdrs, &dummy)  ||
	    	 !xdr_u_long(xdrs, &dummy2) ||
	    	 (dummy != KsRpcAUTHNONE)   || dummy2 ) {
	    	return false;
	    }
	    if ( !xdr_u_long(xdrs, &dummy) ) {
	    	return false;
	    }
	    _accept_status = (KsRpcAcceptStat) dummy;
	    switch ( _accept_status ) {
	    case KsRpcSUCCESS:
	    case KsRpcPROGUNAVAIL:
	    case KsRpcPROCUNAVAIL:
	    case KsRpcGARBAGEARGS:
	    case KsRpcSYSTEMERR:
	    	return true;
	    case KsRpcPROGMISMATCH:
	    	return xdr_u_long(xdrs, &_low) &&
		       xdr_u_long(xdrs, &_high);
	    default:
	    	return false;
	    }
	case KsRpcMSGDENIED:
	    if ( !xdr_u_long(xdrs, &dummy) ) {
	    	return false;
	    }
	    _reject_status = (KsRpcRejectStat) dummy;
	    switch ( _reject_status ) {
	    case KsRpcRPCMISMATCH:
	    	return xdr_u_long(xdrs, &_low) &&
		       xdr_u_long(xdrs, &_high);
	    case KsRpcAUTHERROR:
	    	return xdr_u_long(xdrs, &_auth_error);
	    default:
	    	return false;
	    }
	default:
	    return false;
	}
    //
    // Every other message is just invalid!
    //
    default:
    	return false; // invalid RPC message type
    }
} // KsRpcHeader::xdrDecode


// ---------------------------------------------------------------------------
// Write out (encode) a RPC header to a XDR stream. NB: the pointer casts are
// driving me nuts... but they're needed because this member function is
// declared const and the function prototypes for the XDR functions have
// never heard of this, as they're in and out functions...
//
bool KsRpcHeader::xdrEncode(XDR *xdrs) const
{
    u_long dummy, dummy2;
    
    dummy = (u_long) _msg_type;
    if ( !xdr_u_long(xdrs, (u_long *) &_xid) ||
    	 !xdr_u_long(xdrs, (u_long *) &dummy) ) {
    	return false;
    }
    switch ( _msg_type ) {
    //
    // Send a request (call), thus we're probably on the client side of
    // the connection when we have to serialize (encode) a request.
    //
    case KsRpcCALL:
    	if ( (_rpc_version != 2)                          ||
	     !xdr_u_long(xdrs, (u_long *) &_rpc_version)  ||
	     !xdr_u_long(xdrs, (u_long *) &_prog_number)  ||
	     !xdr_u_long(xdrs, (u_long *) &_prog_version) ||
	     !xdr_u_long(xdrs, (u_long *) &_procedure) ) {
	    return false;
	}
	dummy  = KsRpcAUTHNONE;
	dummy2 = 0;
    	if ( !xdr_u_long(xdrs, &dummy)  ||
	     !xdr_u_long(xdrs, &dummy2) ||
             !xdr_u_long(xdrs, &dummy)  ||
	     !xdr_u_long(xdrs, &dummy2) ) {
	    return false;
	}
    	return true;
    //
    // Send a reply. We're on the server side...
    //
    case KsRpcREPLY:
    	dummy = _reply_status;
    	if ( !xdr_u_long(xdrs, &dummy) ) {
	    return false;
	}
    	switch ( _reply_status ) {
	case KsRpcMSGACCEPTED:
	    dummy  = KsRpcAUTHNONE;
	    dummy2 = 0;
	    if ( !xdr_u_long(xdrs, &dummy) ||
	         !xdr_u_long(xdrs, &dummy2) ) {
	    	return false;
    	    }
	    dummy = _accept_status;
	    if ( !xdr_u_long(xdrs, &dummy) ) {
	    	return false;
	    }
	    switch ( _accept_status ) {
	    case KsRpcSUCCESS:
	    case KsRpcPROGUNAVAIL:
	    case KsRpcPROCUNAVAIL:
	    case KsRpcGARBAGEARGS:
	    case KsRpcSYSTEMERR:
	    	return true;
	    case KsRpcPROGMISMATCH:
	    	return xdr_u_long(xdrs, (u_long *) &_low) &&
		       xdr_u_long(xdrs, (u_long *) &_high);
	    default:
	    	return false;
	    }
	case KsRpcMSGDENIED:
	    dummy = _reject_status;
	    if ( !xdr_u_long(xdrs, &dummy) ) {
	    	return false;
	    }
	    switch ( _reject_status ) {
	    case KsRpcRPCMISMATCH:
	    	return xdr_u_long(xdrs, (u_long *) &_low) &&
		       xdr_u_long(xdrs, (u_long *) &_high);
	    case KsRpcAUTHERROR:
	    	return xdr_u_long(xdrs, (u_long *) &_auth_error);
	    default:
	    	return false;
	    }
	default:
	    return false;
	}
    //
    // Every other message is just invalid!
    //
    default:
    	return false; // invalid RPC message type
    }
} // KsRpcHeader::xdrEncode


void KsRpcHeader::acceptCall()
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcSUCCESS;
} // KsRpcHeader::acceptCall


void KsRpcHeader::setAuthRefusialError(u_long why)
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGDENIED;
    _reject_status = KsRpcAUTHERROR;
    _auth_error    = why;
} // KsRpcHeader::setAuthRefusialError

void KsRpcHeader::setDecodeError()
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcGARBAGEARGS;
} // KsRpcHeader::setDecodeError

void KsRpcHeader::setNoProcedureError()
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcPROCUNAVAIL;
} // KsRpcHeader::setNoProcedureError

void KsRpcHeader::setNoProgrammError()
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcPROGUNAVAIL;
} // KsRpcHeader::setNoProgrammError

void KsRpcHeader::setProgramVersionMismatchError(u_long low, u_long high)
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcPROGMISMATCH;
    _low           = low;
    _high          = high;
} // KsRpcHeader::setProgramVersionMismatchError

void KsRpcHeader::setSystemError()
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcSYSTEMERR;
} // KsRpcHeader::setSystemError

void KsRpcHeader::setRPCVersionMismatchError(u_long low, u_long high)
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGDENIED;
    _reject_status = KsRpcRPCMISMATCH;
    _low           = low;
    _high          = high;
} // KsRpcHeader::setRPCVersionMismatchError

/* End of rpcproto.cpp */