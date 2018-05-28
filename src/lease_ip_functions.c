#include "custom_function.h"

struct lease** ipLeaseList = NULL;
int sizeIpLeaseList;

int readStandartIpLeaseList()
{
    return readIpLease(&ipLeaseList, &sizeIpLeaseList);
}

int checkBusyIp(char* ip)
{
    if (!ipLeaseList) {
        readStandartIpLeaseList();
    }

    for (int i = 0; i < sizeIpLeaseList; i++) {
        if (!strcmp(ip, ipLeaseList[i]->ipaddr)) {
            return 1;
        }
    }

    return 0;
}

int checFileExist(char* filename)
{
    FILE *file;
    if ((file = fopen(filename, "r"))) {
        fclose(file);
        return 1;
    } else {
        return 0;
    } 
}

void freeIpLeaseList()
{
    if (ipLeaseList) {
        for (int i = 0; i < sizeIpLeaseList; i++) {
            free(ipLeaseList[i]);
        }

        free(ipLeaseList);
        sizeIpLeaseList = 0;
        ipLeaseList = NULL;
    }
}

int readIpLease(struct lease*** _ipLeaseList, int* countIp)
{
	FILE* fp;
	char str[100];
	int count = 0;
    struct lease* ipLeaseListTmp[40];
    struct lease* ipLease;

	if ((fp = fopen(LEASE_FILENAME, "r+"))) {
		// error_message("readIpLease(): Error open file for read");
        // return 0;

        while (!feof(fp)) {
            memset(str, 0, sizeof(str));

            if ((fgets(str, 1024, fp) != 0) && (strlen(str) > 16)) {
                ipLease = (struct lease*) malloc(sizeof(struct lease));
                memset(ipLease, 0, sizeof(struct lease));

                strcpy(ipLease->ipaddr, strtok(str, " "));
                strcpy(ipLease->macaddr, strtok(NULL, " "));
                strcpy(ipLease->ttime, strtok(NULL, " "));

                ipLeaseListTmp[count] = ipLease;
                count++;
            }
        }

        fclose(fp);
        *_ipLeaseList = (struct lease**) malloc(sizeof(struct lease) * count);

        for (int i = 0; i < count; i++) {
            _ipLeaseList[0][i] = ipLeaseListTmp[i];
        }
    }

    *countIp = count;

    return 1;
}

int writeIpLease(struct lease* iplease) // Write ip lease in file
{
    if (checkBusyIp(iplease->ipaddr)) {
        error_message("writeIpLease(): IP is used");
        return 0;
    }

	FILE* fp;

	if (!(fp = fopen(LEASE_FILENAME, "a+"))) {
		error_message("writeIpLease(): Error open file for write");
        return 0;	
    }

    time_t t;
	t = time(NULL);
	sprintf(iplease->ttime, "%ld",  t);

    fprintf(fp, "%s %s %s\n", iplease->ipaddr, iplease->macaddr, iplease->ttime);
    fclose(fp);
    freeIpLeaseList();

    return 1;
}

int removeIpFromLeaseList(char* ip)
{   
    readStandartIpLeaseList();
    if (sizeIpLeaseList == 0) {
        return 1;
    }

    // if (checFileExist(LEASE_FILENAME)) {
    //     if (remove(LEASE_FILENAME)) {
    //         error_message("Error remove lease file");
    //         return 0;
    //     }
    // }

    FILE* fp;

	if (!(fp = fopen(LEASE_FILENAME, "w+"))) {
		error_message("removeIpFromLeaseList(): Error open file for write");
        return 0;	
    }

    time_t t = time(NULL);

    for (int i = 0; i < sizeIpLeaseList; i++) {
        if (strcmp(ip, ipLeaseList[i]->ipaddr)) {
            sprintf(ipLeaseList[i]->ttime, "%ld",  t);
            fprintf(fp, "%s %s %s\n", ipLeaseList[i]->ipaddr, ipLeaseList[i]->macaddr, ipLeaseList[i]->ttime);
        }
    }
    fclose(fp);

    freeIpLeaseList();
    
    return 1;
}

