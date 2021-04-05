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
  * Modifided version for MAC GPU MINER
  * By Garry Lachman 2021
  *
  */

// compile with
// -fvisibility=hidden

#include "xmrstak/backend/backendConnector.hpp"
#include "xmrstak/backend/globalStates.hpp"
#include "xmrstak/backend/miner_work.hpp"
#include "xmrstak/donate-level.hpp"
#include "xmrstak/jconf.hpp"
#include "xmrstak/misc/configEditor.hpp"
#include "xmrstak/misc/console.hpp"
#include "xmrstak/misc/executor.hpp"
#include "xmrstak/misc/utility.hpp"
#include "xmrstak/params.hpp"
#include "xmrstak/version.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

#include "xmrstak/misc/event.hpp"
#include "lib-miner.h"

// Initializer.
__attribute__((constructor))
static void initializer(void) {
    printf("[%s] initializer()\n", __FILE__);
}
 
// Finalizer.
__attribute__((destructor))
static void finalizer(void) {
    printf("[%s] finalizer()\n", __FILE__);
}

EXPORT
event* get_eventter() {
    return event::inst();
}

EXPORT
RETURN_CODES start_miner(std::string poolURL, std::string poolUsername, std::string poolPasswd)
{

	srand(time(0));
    
	using namespace xmrstak;

	bool pool_url_set = false;

    params::inst().useCPU = false;
    params::inst().useAMD = true;
    params::inst().useNVIDIA = false;
    //params::inst().rootAMDCacheDir
    params::inst().currency = "cryptonight_gpu";
    params::inst().poolURL = poolURL;
    params::inst().poolUseTls = false;
    params::inst().poolUsername = poolUsername;
    params::inst().userSetPwd = true;
    params::inst().poolPasswd = poolPasswd;
    params::inst().userSetRigid = false;
    params::inst().poolRigid = "";
    params::inst().nicehashMode = false;
    //params::inst().configFile
    //params::inst().outputFile
    params::inst().h_print_time = 30;
    //params::inst().benchmark_block_version = bversion;
    //params::inst().benchmark_wait_sec = waitsec;
    //params::inst().benchmark_work_sec
    

	//bool hasConfigFile = configEditor::file_exist(params::inst().configFile);
	//bool hasPoolConfig = configEditor::file_exist(params::inst().configFilePools);

    if (!jconf::inst()->parse_params()) {
        return RETURN_CODES::CONFIG_PARSE_ERROR;
    }

	//if(!jconf::inst()->parse_config(params::inst().configFile.c_str(), params::inst().configFilePools.c_str()))
	//{
        //return RETURN_CODES::CONFIG_PARSE_ERROR;
	//}
    
	if(strlen(jconf::inst()->GetOutputFile()) != 0)
		printer::inst()->open_logfile(jconf::inst()->GetOutputFile());

	if(!BackendConnector::self_test())
	{
		printer::inst()->print_msg(L0, "Self test not passed!");
        return RETURN_CODES::SELF_TEST_ERROR;
	}

	printer::inst()->print_msg(L0, "Mining coin: %s", ::jconf::inst()->GetCurrentCoinSelection().GetDescription(1).GetMiningAlgo().Name().c_str());


	executor::newInst()->ex_start(jconf::inst()->DaemonMode());
    
    //executor::inst()->push_event(ex_event(EV_USR_HASHRATE));
    //executor::inst()->push_event(ex_event(EV_USR_RESULTS));
    //executor::inst()->push_event(ex_event(EV_USR_CONNSTAT));

	/*while(true)
	{
		//event loop??
	}*/

	return RETURN_CODES::EXIT;
}

EXPORT
std::vector<double> get_total_hashrate() {
    return executor::inst()->get_total_hashrate();
}

EXPORT
double get_top_hashrate() {
    return executor::inst()->get_top_hashrate();
}

EXPORT
void stop_miner() {
    executor::inst()->ex_stop();
}
