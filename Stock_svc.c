/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "Stock.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
validation_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		data1 validation_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case Validation:
		_xdr_argument = (xdrproc_t) xdr_data1;
		_xdr_result = (xdrproc_t) xdr_reponse1;
		local = (char *(*)(char *, struct svc_req *)) validation_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

static void
distribution_2(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		data2 distribution_2_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case distribution:
		_xdr_argument = (xdrproc_t) xdr_data2;
		_xdr_result = (xdrproc_t) xdr_reponse2;
		local = (char *(*)(char *, struct svc_req *)) distribution_2_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (Validation, VERSION_UN);
	pmap_unset (distribution, VERSION_UN);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, Validation, VERSION_UN, validation_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (Validation, VERSION_UN, udp).");
		exit(1);
	}
	if (!svc_register(transp, distribution, VERSION_UN, distribution_2, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (distribution, VERSION_UN, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, Validation, VERSION_UN, validation_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (Validation, VERSION_UN, tcp).");
		exit(1);
	}
	if (!svc_register(transp, distribution, VERSION_UN, distribution_2, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (distribution, VERSION_UN, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
