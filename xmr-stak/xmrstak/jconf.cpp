/*
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * Additional permission under GNU GPL version 3 section 7
  *
  * If you modify this Program, or any covered work, by linking or combining
  * it with OpenSSL (or a modified version of that library), containing parts
  * covered by the terms of OpenSSL License and SSLeay License, the licensors
  * of this Program grant you additional permission to convey the resulting work.
  *
  */

#include "jconf.hpp"
#include "params.hpp"

#include "xmrstak/misc/console.hpp"
#include "xmrstak/misc/jext.hpp"
#include "xmrstak/misc/utility.hpp"

#include <algorithm>
#include <math.h>
#include <numeric>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#ifdef _WIN32
#define strcasecmp _stricmp
#include <intrin.h>
#else
#include <cpuid.h>
#endif

using namespace rapidjson;

/*
 * This enum needs to match index in oConfigValues, otherwise we will get a runtime error
 */
enum configEnum
{
	aPoolList,
	sCurrency,
	bTlsSecureAlgo,
	iCallTimeout,
	iNetRetry,
	iGiveUpLimit,
	iVerboseLevel,
	bPrintMotd,
	iAutohashTime,
	bDaemonMode,
	sOutputFile,
	iHttpdPort,
	sHttpLogin,
	sHttpPass,
	bPreferIpv4,
	bAesOverride,
	sUseSlowMem
};

struct configVal
{
	configEnum iName;
	const char* sName;
	Type iType;
};

// Same order as in configEnum, as per comment above
// kNullType means any type
configVal oConfigValues[] = {
	{aPoolList, "pool_list", kArrayType},
	{sCurrency, "currency", kStringType},
	{bTlsSecureAlgo, "tls_secure_algo", kTrueType},
	{iCallTimeout, "call_timeout", kNumberType},
	{iNetRetry, "retry_time", kNumberType},
	{iGiveUpLimit, "giveup_limit", kNumberType},
	{iVerboseLevel, "verbose_level", kNumberType},
	{bPrintMotd, "print_motd", kTrueType},
	{iAutohashTime, "h_print_time", kNumberType},
	{bDaemonMode, "daemon_mode", kTrueType},
	{sOutputFile, "output_file", kStringType},
	{iHttpdPort, "httpd_port", kNumberType},
	{sHttpLogin, "http_login", kStringType},
	{sHttpPass, "http_pass", kStringType},
	{bPreferIpv4, "prefer_ipv4", kTrueType},
	{bAesOverride, "aes_override", kNullType},
	{sUseSlowMem, "use_slow_memory", kStringType}};

constexpr size_t iConfigCnt = (sizeof(oConfigValues) / sizeof(oConfigValues[0]));

xmrstak::coin_selection coins[] = {
	// name, userpool, devpool, default_pool_suggestion
	{"bbscoin", {POW(cryptonight_aeon)}, {POW(cryptonight_aeon)}, nullptr},
	{"bittube", {POW(cryptonight_bittube2)}, {POW(cryptonight_gpu)}, "mining.bit.tube:13333"},
	{"cryptonight", {POW(cryptonight)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_bittube2", {POW(cryptonight_bittube2)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_masari", {POW(cryptonight_masari)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_haven", {POW(cryptonight_haven)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_heavy", {POW(cryptonight_heavy)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_lite", {POW(cryptonight_lite)}, {POW(cryptonight_aeon)}, nullptr},
	{"cryptonight_lite_v7", {POW(cryptonight_aeon)}, {POW(cryptonight_aeon)}, nullptr},
	{"cryptonight_lite_v7_xor", {POW(cryptonight_ipbc)}, {POW(cryptonight_aeon)}, nullptr},
	{"cryptonight_r", {POW(cryptonight_r)}, {POW(cryptonight_r)}, nullptr},
	{"cryptonight_superfast", {POW(cryptonight_superfast)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_turtle", {POW(cryptonight_turtle)}, {POW(cryptonight_turtle)}, nullptr},
	{"cryptonight_v7", {POW(cryptonight_monero)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_v8", {POW(cryptonight_monero_v8)}, {POW(cryptonight_r)}, nullptr},
	{"cryptonight_v8_double", {POW(cryptonight_v8_double)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_v8_half", {POW(cryptonight_v8_half)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_v8_reversewaltz", {POW(cryptonight_v8_reversewaltz)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_v8_zelerius", {POW(cryptonight_v8_zelerius)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_v7_stellite", {POW(cryptonight_stellite)}, {POW(cryptonight_gpu)}, nullptr},
	{"cryptonight_gpu", {POW(cryptonight_gpu)}, {POW(cryptonight_gpu)}, "pool.ryo-currency.com:3333"},
	{"cryptonight_conceal", {POW(cryptonight_conceal)}, {POW(cryptonight_gpu)}, nullptr},
	{"graft", {POW(cryptonight_v8_reversewaltz), 12, POW(cryptonight_monero_v8)}, {POW(cryptonight_gpu)}, nullptr},
	{"haven", {POW(cryptonight_haven)}, {POW(cryptonight_gpu)}, nullptr},
	{"lethean", {POW(cryptonight_r)}, {POW(cryptonight_r)}, nullptr},
	{"masari", {POW(cryptonight_v8_half)}, {POW(cryptonight_gpu)}, nullptr},
	{"qrl", {POW(cryptonight_monero)}, {POW(cryptonight_gpu)}, nullptr},
	{"ryo", {POW(cryptonight_gpu)}, {POW(cryptonight_gpu)}, "pool.ryo-currency.com:3333"},
	{"torque", {POW(cryptonight_v8_half)}, {POW(cryptonight_gpu)}, nullptr},
	{"plenteum", {POW(cryptonight_turtle)}, {POW(cryptonight_turtle)}, nullptr},
	{"zelerius", {POW(cryptonight_v8_zelerius), 7, POW(cryptonight_monero_v8)}, {POW(cryptonight_gpu)}, nullptr}};

constexpr size_t coin_algo_size = (sizeof(coins) / sizeof(coins[0]));

inline bool checkType(Type have, Type want)
{
	if(want == have)
		return true;
	else if(want == kNullType)
		return true;
	else if(want == kTrueType && have == kFalseType)
		return true;
	else if(want == kFalseType && have == kTrueType)
		return true;
	else
		return false;
}

struct jconf::opaque_private
{
	Document jsonDoc;
	Document jsonDocPools;
	const Value* configValues[iConfigCnt]; //Compile time constant

	opaque_private()
	{
	}
};

jconf::jconf()
{
	prv = new opaque_private();
}

uint64_t jconf::GetPoolCount()
{
    return 1;
}

bool jconf::GetPoolConfig(size_t id, pool_cfg& cfg)
{
    cfg.sPoolAddr = params::inst().poolURL.c_str();
    cfg.sWalletAddr = params::inst().poolUsername.c_str();
    cfg.sRigId = params::inst().poolRigid.c_str();
    cfg.sPasswd = params::inst().poolPasswd.c_str();
    cfg.nicehash = params::inst().nicehashMode;
    cfg.tls = params::inst().poolUseTls;
    cfg.tls_fingerprint = "";
    cfg.raw_weight = 1;

	size_t dlt = wt_max - wt_min;
	if(dlt != 0)
	{
		/* Normalise weights between 0 and 9.8 */
		cfg.weight = double(cfg.raw_weight - wt_min) * 9.8;
		cfg.weight /= dlt;
	}
	else /* Special case - user selected same weights for everything */
		cfg.weight = 0.0;
	return true;
}

bool jconf::TlsSecureAlgos()
{
    return params::inst().poolUseTls;
}

bool jconf::PreferIpv4()
{
    return true;
}

uint64_t jconf::GetCallTimeout()
{
    return 10;
}

uint64_t jconf::GetNetRetry()
{
    return 30;
}

uint64_t jconf::GetGiveUpLimit()
{
    return 5;
}

uint64_t jconf::GetVerboseLevel()
{
    return 10;
}

bool jconf::PrintMotd()
{
    return true;
}

uint64_t jconf::GetAutohashTime()
{
    return uint64_t(params::inst().h_print_time);
}

uint16_t jconf::GetHttpdPort()
{
    return 8888;
}

const char* jconf::GetHttpUsername()
{
    return "";
}

const char* jconf::GetHttpPassword()
{
    return "";
}

bool jconf::DaemonMode()
{
    return false;
}

const char* jconf::GetOutputFile()
{
    return params::inst().outputFile.c_str();
}

void jconf::cpuid(uint32_t eax, int32_t ecx, int32_t val[4])
{
	memset(val, 0, sizeof(int32_t) * 4);

#ifdef _WIN32
	__cpuidex(val, eax, ecx);
#else
	__cpuid_count(eax, ecx, val[0], val[1], val[2], val[3]);
#endif
}

bool jconf::check_cpu_features()
{
	constexpr int AESNI_BIT = 1 << 25;
	constexpr int SSE2_BIT = 1 << 26;
	int32_t cpu_info[4];
	bool bHaveSse2;

	cpuid(1, 0, cpu_info);

	bHaveAes = (cpu_info[2] & AESNI_BIT) != 0;
	bHaveSse2 = (cpu_info[3] & SSE2_BIT) != 0;

	return bHaveSse2;
}

jconf::slow_mem_cfg jconf::GetSlowMemSetting()
{
    return print_warning;
}

std::string jconf::GetMiningCoin()
{
    return xmrstak::params::inst().currency;
}

void jconf::GetAlgoList(std::string& list)
{
	list.reserve(256);
	for(size_t i = 0; i < coin_algo_size; i++)
	{
		list += "\t- ";
		list += coins[i].coin_name;
		list += "\n";
	}
}

bool jconf::IsOnAlgoList(std::string& needle)
{
	std::transform(needle.begin(), needle.end(), needle.begin(), ::tolower);

	for(size_t i = 0; i < coin_algo_size; i++)
	{
		if(needle == coins[i].coin_name)
			return true;
	}
	return false;
}

const char* jconf::GetDefaultPool(const char* needle)
{
	const char* default_example = "pool.example.com:3333";

	for(size_t i = 0; i < coin_algo_size; i++)
	{
		if(strcmp(needle, coins[i].coin_name) == 0)
		{
			if(coins[i].default_pool != nullptr)
				return coins[i].default_pool;
			else
				return default_example;
		}
	}

	return default_example;
}

bool jconf::parse_params()
{
    std::vector<size_t> pool_weights;
    
    pool_weights.emplace_back(1);
    wt_max = *std::max_element(pool_weights.begin(), pool_weights.end());
    wt_min = *std::min_element(pool_weights.begin(), pool_weights.end());
    
    bHaveAes = false;
    printer::inst()->set_verbose_level(1);
    
    std::string ctmp = GetMiningCoin();
    std::transform(ctmp.begin(), ctmp.end(), ctmp.begin(), ::tolower);

    printer::inst()->print_msg(L0, "Coin %s", ctmp.c_str());
    if(ctmp.length() == 0)
    {
        printer::inst()->print_msg(L0, "You need to specify the coin that you want to mine.");
        return false;
    }
    
    for(size_t i = 0; i < coin_algo_size; i++)
    {
        if(ctmp == coins[i].coin_name)
        {
            currentCoin = coins[i];
            break;
        }
    }
    
    if(currentCoin.GetDescription(1).GetMiningAlgo() == invalid_algo)
    {
        std::string cl;
        GetAlgoList(cl);
        printer::inst()->print_msg(L0, "Unrecognised coin '%s', your options are:\n%s", ctmp.c_str(), cl.c_str());
        return false;
    }
    
    return true;
    
}
