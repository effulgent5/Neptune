#include "kiMessageClient.h"

kiMessageClient::kiMessageClient()
{

}

kiMessageClient::~kiMessageClient()
{

}

kiMessageClient::eMsgResult kiMessageClient::onMsg(UINT msg, VOID* data)
{
	return MR_PAS_CTU;
}