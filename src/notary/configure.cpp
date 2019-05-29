
#include <utils/utils.h>
#include <utils/file.h>
#include <utils/strings.h>
#include <utils/logger.h>
#include <common/general.h>
#include "configure.h"

namespace bubi {

	NotaryConfigure::NotaryConfigure() {
		enabled_ = false;
	}

	NotaryConfigure::~NotaryConfigure() {
	}

	bool NotaryConfigure::Load(const Json::Value &value) {
		Configure::GetValue(value, "enabled", enabled_);

		std::string address;
		Configure::GetValue(value, "notary_addr", address);
		listen_addr_ = utils::InetAddress(address);

		return true;
	}

	bool PairChainConfigure::Load(const Json::Value &value) {
		Configure::GetValue(value, "chain_unique", chain_unique_);
		Configure::GetValue(value, "target_chain_unique", target_chain_unique_);
		Configure::GetValue(value, "comm_contract", comm_contract_);
		return true;
	}

	Configure::Configure() {}

	Configure::~Configure() {}

	bool Configure::LoadFromJson(const Json::Value &values){
		if (!values.isMember("notary") ||
			!values.isMember("logger")) {
			LOG_STD_ERR("Some configuration not exist");
			return false;
		}

		logger_configure_.Load(values["logger"]);
		notary_configure_.Load(values["notary"]);

		PairChainConfigure chain1;
		chain1.Load(values["pair_chain_1"]);
		PairChainConfigure chain2;
		chain2.Load(values["pair_chain_2"]);

		if (chain1.chain_unique_ != chain2.target_chain_unique_){
			return false;
		}

		if (chain1.target_chain_unique_ != chain2.chain_unique_){
			return false;
		}

		pair_chain_map_[chain1.chain_unique_] = chain1;
		pair_chain_map_[chain2.chain_unique_] = chain2;

		return true;
	}
}
