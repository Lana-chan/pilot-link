/*
 * addresses.c:  Translate Palm address book into a generic format
 *
 * Copyright (c) 1996, Kenneth Albanowski
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include "popt.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pi-socket.h"
#include "pi-address.h"
#include "pi-dlp.h"
#include "pi-header.h"

const char *port        = NULL;

const struct poptOption options[] = {
        { "port",    'p', POPT_ARG_STRING, &port, 0, "Use device <port> to communicate with Palm", "<port>"},
        { "help",    'h', POPT_ARG_NONE,   0, 'h', "Display this information"},
        { "version", 'v', POPT_ARG_NONE,   0, 'v', "Display version information"},
        POPT_AUTOHELP
        { NULL, 0, 0, NULL, 0 }
};

poptContext po;

/***********************************************************************
 *
 * Function:    display_help
 *
 * Summary:     Print out the --help options and arguments  
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 ***********************************************************************/
static void display_help(const char *progname)
{
	printf("Dumps the Palm AddressDB database into a generic text output format\n\n");

	poptPrintHelp(po, stderr, 0);

	printf("\nOnly the port option is required, the other options are... optional.\n\n");
	printf("Example:\n");
	printf("  %s -p /dev/pilot\n\n", progname);
	printf("You can redirect the output of 'addresses' to a file instead of the default\n");
	printf("STDOUT by using redirection and pipes as necessary.\n\n");
	printf("Example:\n");
	printf("  %s -p /dev/pilot > MyAddresses.txt\n\n", progname);

	return;
}

int main(int argc, char *argv[])
{
	int 	db,
		index,
		sd 		= -1,
		po_err		= -1;
	
	char 	*progname 	= argv[0];
	
	struct 	AddressAppInfo aai;

	pi_buffer_t *buffer;
	
        po = poptGetContext("addresses", argc, (const char **) argv, options, 0);
  
        if (argc < 2) {
                display_help(progname);
                exit(1);
        }
  
        while ((po_err = poptGetNextOpt(po)) != -1) {
                switch (po_err) {

		case 'v':
			print_splash(progname);
			return 0;
		default:
			poptPrintHelp(po, stderr, 0);
			return 0;
                }
        }

	sd = pilot_connect(port);

	if (sd < 0)
		goto error;

	if (dlp_OpenConduit(sd) < 0)
		goto error_close;
	
	/* Open the Address book's database, store access handle in db */
	if (dlp_OpenDB(sd, 0, 0x80 | 0x40, "AddressDB", &db) < 0) {
		puts("Unable to open AddressDB");
		dlp_AddSyncLogEntry(sd, "Unable to open AddressDB.\n");
		exit(EXIT_FAILURE);
	}
	
	buffer = pi_buffer_new (0xffff);
	
	dlp_ReadAppBlock(sd, db, 0, buffer->data, 0xffff);
	unpack_AddressAppInfo(&aai, buffer->data, 0xffff);
	
	for (index = 0;; index++) {
		int 	i,
			attr,
			category;

		struct 	Address addr;

		int len =
		    dlp_ReadRecordByIndex(sd, db, index, buffer, 0, &attr,
					  &category);
	
		if (len < 0)
			break;
	
		/* Skip deleted records */
		if ((attr & dlpRecAttrDeleted)
		    || (attr & dlpRecAttrArchived))
			continue;
	
		unpack_Address(&addr, buffer->data, buffer->used);

		printf("Category: %s\n", aai.category.name[category]);

		for (i = 0; i < 19; i++) {
			if (addr.entry[i]) {
				int l = i;
	
				if ((l >= entryPhone1) && (l <= entryPhone5)) {
					printf("%s: %s\n", 
						aai.phoneLabels[addr.phoneLabel[l - entryPhone1]], 
						addr.entry[i]);
				} else {
					printf("%s: %s\n", aai.labels[l], 
						addr.entry[i]);
				}
			}
		}
		printf("\n");
		free_Address(&addr);
	}

	pi_buffer_free (buffer);

	/* Close the database */
	dlp_CloseDB(sd, db);
        dlp_AddSyncLogEntry(sd, "Successfully read addresses from Palm.\n\n"
				"Thank you for using pilot-link.\n");
	dlp_EndOfSync(sd, 0);
	pi_close(sd);
	return 0;

 error_close:
        pi_close(sd);

 error:
        return -1;
}

/* vi: set ts=8 sw=4 sts=4 noexpandtab: cin */
