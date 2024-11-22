#include <API/Challenge/Manager.h>

int main() {
    ChallengesService* service = NEW(ChallengeManager);



    free(service);
    return 0;
}
