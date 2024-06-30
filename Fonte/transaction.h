#define FTRANSACTION 1

void startTransaction();
void commitTransaction();
void rollbackTransaction();

int isTransactionInProgress();

void gotError();

int cloneRepository();
int discardChanges();
int applyChanges();