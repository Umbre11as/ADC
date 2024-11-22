#include "Manager.h"

ChallengesService* NewChallengeManager() {
    ChallengesService* manager = SALLOC(ChallengesService);

    return manager;
}
