#include <stdio.h>

int isTransactionOpened = 0;

void startTransaction()
{

  if (isTransactionOpened)
  {
    printf("ERROR: a transaction is oppened.\n");
    return;
  }

  isTransactionOpened = 1;

  printf("beggin command!\n");
}

void commitTransaction()
{
  printf("commit command!\n");
}

void rollbackTransaction()
{
  printf("rollback command!\n");
}

void cloneRepository(){}

void discardChanges(){}

void applyChanges(){}