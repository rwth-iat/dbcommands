/* -*-plt-c++-*- */
/*
 * $Header: /home/david/cvs/acplt/ks/src/objmgrparams.cpp,v 1.1 1998-10-06 13:21:28 harald Exp $
 *
 * Copyright (c) 1996, 1997, 1998
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

/* Author: Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "ks/objmgrparams.h"


// ---------------------------------------------------------------------------
// Implement the constructor, factory and encoding/decoding stuff for the
// KsPlacementHint class.
//
KS_IMPL_XDRNEW(KsPlacementHint);
KS_IMPL_XDRCTOR(KsPlacementHint);


bool
KsPlacementHint::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return ks_xdre_enum(xdr, &hint)
        && ((hint == KS_PMH_BEFORE) ||
            (hint == KS_PMH_AFTER) ?  place_path.xdrEncode(xdr) : true);
} // KsPlacementHint::xdrEncode


bool
KsPlacementHint::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return ks_xdrd_enum(xdr, &hint)
        && ((hint == KS_PMH_BEFORE) ||
            (hint == KS_PMH_AFTER) ? place_path.xdrDecode(xdr) : true);
} // KsPlacementHint::xdrDecode


// ---------------------------------------------------------------------------
// Implement the constructor, factory and encoding/decoding stuff for the
// KsLinkItem class.
//
KS_IMPL_XDRNEW(KsLinkItem);
KS_IMPL_XDRCTOR(KsLinkItem);


bool
KsLinkItem::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return link_path.xdrEncode(xdr)
        && new_member_path.xdrEncode(xdr)
        && place.xdrEncode(xdr);
} // KsLinkItem::xdrEncode


bool
KsLinkItem::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return link_path.xdrDecode(xdr)
        && new_member_path.xdrDecode(xdr)
        && place.xdrDecode(xdr);
} // KsLinkItem::xdrDecode


// ---------------------------------------------------------------------------
//
KS_IMPL_XDRNEW(KsCreateObjItem);
KS_IMPL_XDRCTOR(KsCreateObjItem);


bool
KsCreateObjItem::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return factory_path.xdrEncode(xdr)
        && new_path.xdrEncode(xdr)
        && place.xdrEncode(xdr)
        && parameters.xdrEncode(xdr)
        && links.xdrEncode(xdr);
} // KsCreateObjItem::xdrEncode


bool
KsCreateObjItem::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return factory_path.xdrDecode(xdr)
        && new_path.xdrDecode(xdr)
        && place.xdrDecode(xdr)
        && parameters.xdrDecode(xdr)
        && links.xdrDecode(xdr);
} // KsCreateObjItem::xdrDecode


// ---------------------------------------------------------------------------
// Implement the constructor, factory and encoding/decoding stuff for the
// CreateObject service parameter class.
//
KS_IMPL_XDRNEW(KsCreateObjParams);
KS_IMPL_XDRCTOR(KsCreateObjParams);


bool
KsCreateObjParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return items.xdrEncode(xdr);
} // KsCreateObjParams::xdrEncode


bool
KsCreateObjParams::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return items.xdrDecode(xdr);
} // KsCreateObjParams::xdrDecode


//
KS_IMPL_XDRNEW(KsCreateObjResult);
KS_IMPL_XDRCTOR(KsCreateObjResult);


bool
KsCreateObjResult::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return KsResult::xdrEncode(xdr)
        && (result == KS_ERR_BADINITPARAM ?
	           param_results.xdrEncode(xdr)
	        && link_results.xdrEncode(xdr)
	    : true);
} // KsCreateObjResult::xdrEncode


bool
KsCreateObjResult::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return KsResult::xdrDecode(xdr)
        && (result == KS_ERR_BADINITPARAM ?
	           param_results.xdrDecode(xdr)
	        && link_results.xdrDecode(xdr)
	    : true);
} // KsCreateObjResult::xdrDecode


// ---------------------------------------------------------------------------
//
KS_IMPL_XDRNEW(KsDeleteObjParams);
KS_IMPL_XDRCTOR(KsDeleteObjParams);


bool
KsDeleteObjParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return paths.xdrEncode(xdr);
} // KsDeleteObjParams::xdrEncode


bool
KsDeleteObjParams::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return paths.xdrDecode(xdr);
} // KsDeleteObjParams::xdrDecode


KS_IMPL_XDRNEW(KsDeleteObjResult);
KS_IMPL_XDRCTOR(KsDeleteObjResult);


bool
KsDeleteObjResult::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return KsResult::xdrEncode(xdr)
        && results.xdrEncode(xdr);
} // KsDeleteObjResult::xdrEncode


bool
KsDeleteObjResult::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return KsResult::xdrDecode(xdr)
        && results.xdrDecode(xdr);
} // KsDeleteObjResult::xdrDecode


/* End of ks/objmgrparams.cpp */