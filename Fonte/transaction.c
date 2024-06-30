#include <stdio.h>
#include <stdlib.h>
#include "transaction.h"
#include "directory_utils.h"

int isTransactionOpened = 0;
int gotAnyError = 0;

void startTransaction()
{
  if (isTransactionOpened)
  {
    printf("WARNING: there is already a transaction in progress.\n");
    printf("BEGIN\n");
    return;
  }

  if (!cloneRepository())
  {
    printf("ERROR: failed to initialize transaction.\n");
    return;
  }

  isTransactionOpened = 1;
  gotAnyError = 0;
  deleteDirectory("./databackup");
  printf("BEGIN\n");
}

void commitTransaction()
{
  if (!isTransactionOpened)
  {
    printf("WARNING: there is no transaction in progress\n");
    printf("COMMIT\n");
    return;
  }

  if (gotAnyError)
  {
    rollbackTransaction();
    return;
  }

  if (!applyChanges())
  {
    printf("ERROR: failed to commit transaction.\n");
    return;
  }

  isTransactionOpened = 0;
  gotAnyError = 0;
  printf("COMMIT\n");
}

void rollbackTransaction()
{
  if (!isTransactionOpened)
  {
    printf("WARNING: there is no transaction in progress\n");
    printf("ROLLBACK\n");
    return;
  }

  if (!discardChanges())
  {
    printf("ERROR: failed to rollback transaction.\n");
    return;
  }

  isTransactionOpened = 0;
  gotAnyError = 0;
  printf("ROLLBACK\n");
}

void gotError()
{
  gotAnyError = 1;
}

int cloneRepository()
{
  if (directoryExists("./copy") && !deleteDirectory("./copy"))
  {
    printf("ERROR: failed to initialize transaction.\n");
    return 0;
  }

  if (!copyDirectory("./data", "./copy"))
  {
    printf("ERROR: failed to initialize transaction.\n");
    return 0;
  }

  return 1;
}

int isTransactionInProgress()
{
  return isTransactionOpened;
}

int discardChanges()
{
  if (!renameDirectory("./data", "./databackup"))
  {
    renameDirectory("./databackup", "./data");
    return 0;
  }

  if (!renameDirectory("./copy", "./data"))
  {
    renameDirectory("./databackup", "./data");
    return 0;
  }

  deleteDirectory("./databackup");
  return 1;
}

int applyChanges()
{
  return deleteDirectory("./copy");
}