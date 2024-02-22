#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NAME_LENGTH_MAXIMUM 10
#define TRANSACTIONS_LENGTH_MAXIMUM 255
#define ACCOUNTS_MAXIMUM 100
#define CLIENTS_MAXIMUM 100

pthread_mutex_t accountLocks[ACCOUNTS_MAXIMUM];
int accountBalances[ACCOUNTS_MAXIMUM];
char accountNames[ACCOUNTS_MAXIMUM][NAME_LENGTH_MAXIMUM];
int totalAccounts = 0;

char clientTransactions[CLIENTS_MAXIMUM][TRANSACTIONS_LENGTH_MAXIMUM];
char clientNames[CLIENTS_MAXIMUM][NAME_LENGTH_MAXIMUM];
int totalClients = 0;

void *computeTransaction(void *currentClient)
{
    int accountNumber;
    char *block;
    char *quantity;
    char *user = (char *)currentClient;

    block = strtok(user, " ");

    while (block != NULL)
    {
        if (strcmp(block, "deposit") == 0)
        {
            char *accountUser = strtok(NULL, " ");
            for (int k = 0; totalAccounts > k; k++)
            {
                if (strcmp(accountNames[k], accountUser) == 0)
                {
                    accountNumber = k;
                }
            }
            pthread_mutex_lock(&accountLocks[accountNumber]);
            quantity = strtok(NULL, " ");
            accountBalances[accountNumber] = accountBalances[accountNumber] + atoi(quantity);
            pthread_mutex_unlock(&accountLocks[accountNumber]);
        }
        else if (strcmp(block, "withdraw") == 0)
        {
            char *accountUser = strtok(NULL, " ");
            for (int j = 0; totalAccounts > j; j++)
            {
                if (strcmp(accountNames[j], accountUser) == 0)
                {
                    accountNumber = j;
                }
            }
            pthread_mutex_lock(&accountLocks[accountNumber]);
            quantity = strtok(NULL, " ");
            if ((0 <= accountBalances[accountNumber] - atoi(quantity)))
            {
                accountBalances[accountNumber] = accountBalances[accountNumber] - atoi(quantity);
            }
            pthread_mutex_unlock(&accountLocks[accountNumber]);
        }
        block = strtok(NULL, " ");
    }
    return 0;
}

int main()
{
    void *out;
    char row[TRANSACTIONS_LENGTH_MAXIMUM];

    char fileInput[] = "assignment_5_input.txt";
    FILE *f = fopen(fileInput, "r");

    while (fgets(row, sizeof(row), f) != NULL)
    {
        if (row[0] == 'C')
        {
            totalClients = totalClients + 1;
        }
        else if (row[0] == 'A')
        {
            totalAccounts = totalAccounts + 1;
        }
    }

    printf("No. of Accounts: %d\n", totalAccounts);
    printf("No. of Clients: %d\n", totalClients);

    fseek(f, 0, SEEK_SET);

    for (int x = 0; totalAccounts > x; x++)
    {
        fgets(row, sizeof(row), f);
        char *block = strtok(row, " ");

        strncpy(accountNames[x], block, NAME_LENGTH_MAXIMUM);

        for (int y = 0; 2 > y; y++)
        {
            block = strtok(NULL, " ");
            if (y == 1)
            {
                accountBalances[x] = atoi(block);
            }
        }

        pthread_mutex_init(&accountLocks[x], NULL);
    }

    pthread_t threads[totalClients];

    for (int w = 0; totalClients > w; w++)
    {
        char *block = strtok(row, " ");
        fgets(row, sizeof(row), f);
        strncpy(clientNames[w], block, NAME_LENGTH_MAXIMUM);
        block = strtok(NULL, "\n");
        strncpy(clientTransactions[w], block, TRANSACTIONS_LENGTH_MAXIMUM);

        if (pthread_create(&threads[w], NULL, computeTransaction, (void *)clientTransactions[w]) != 0)
        {
            printf("Error while creating thread %d\n", w);
            exit(2);
        }

        if (pthread_join(threads[w], &out) != 0)
        {
            perror("pthread_join() error on thread");
            exit(2);
        }
    }

    fclose(f);

    for (int n = 0; totalAccounts > n; n++)
    {
        printf("%s balance %d\n", accountNames[n], accountBalances[n]);
    }

    for (int p = 0; totalAccounts > p; ++p)
    {
        pthread_mutex_destroy(&accountLocks[p]);
    }

    return 0;
}

