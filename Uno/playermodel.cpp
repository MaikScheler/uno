#include "playermodel.h"

PlayerModel::PlayerModel(int clientId, bool primary)
{
    this->clientId = clientId;
    this->primary = primary;
}

int PlayerModel::getClientId() {
    return this->clientId;
}

bool PlayerModel::getPrimary() {
    return this->primary;
}
