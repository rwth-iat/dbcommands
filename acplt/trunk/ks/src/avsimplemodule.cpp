/* -*-plt-c++-*- */

/*
 * Copyright (c) 1996, 1997
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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include "ks/avsimplemodule.h"

//////////////////////////////////////////////////////////////////////

bool
KscSimpleNegotiator::xdrEncode(XDR *xdr)
{
    enum_t auth_simple = KS_AUTH_SIMPLE;
    return ks_xdre_enum(xdr, &auth_simple)
        && id.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool
KscSimpleNegotiator::xdrDecode(XDR *xdr)
{
    enum_t auth_type;

    bool ok = ks_xdrd_enum(xdr, &auth_type);

    return ok &&
        auth_type == KS_AUTH_SIMPLE;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool 
KscAvSimpleModule::addId(const PltString &server, 
                         const PltString &identification)
{
    PltString dummy_string;
    bool dummy_bool;

    return _id_table.update(server, identification,
                            dummy_string, dummy_bool);
}

//////////////////////////////////////////////////////////////////////

KscNegotiatorHandle
KscAvSimpleModule::getNegotiator(const KscServer *server) const
{
    KsString server_name(server->getHostAndName());
    KsString id;

    if(_id_table.query(server_name, id)) {
        return KscNegotiatorHandle(
            new KscSimpleNegotiator(id), 
            KsOsNew);
    } else {
        return KscNegotiatorHandle(
            KscAvNoneModule::getStaticNegotiator(),
            KsOsUnmanaged);
    }
}

//////////////////////////////////////////////////////////////////////
// EOF avmodule.cpp
//////////////////////////////////////////////////////////////////////



