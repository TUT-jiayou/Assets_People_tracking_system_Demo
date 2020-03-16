# demo-Assets People tracking system

## 1. Preparation
### 1.1 Demo program recurrence steps
* Purchase T95 development board
* Download the SDK and guidance documents of T95
* Merge Source code
* Compile new firmware
* Firmware burning
* Program effect demonstration

### 1.2 How to  get T95's  SDK
Sample_Applications can get SDK by purchasing T95 development kit
Link : [Thundercomm T95](https://www.thundercomm.com/app_en/store/detail/1577783630023328)

### 1.3 How to build and debug T95's hardware firmware
This article summarizes the compilation and debugging steps. For detailed steps, please refer to the document 《Thundercomm+TurboX+T95+Platform+IoT+SDK+User+Guide.pdf》
* [Python](https://www.python.org/)
> Python is used to compile the sample in SDK, please install Python 2.7.9 or more advanced. 
* [LLVM 4.0 compiler](https://developer.qualcomm.com/software/snapdragon-llvm-compiler-android/tools)
> LLVM is used to compile the sample in SDK
* Build app firework
```shell
SDK\Sample_Applications\io9205_led> .\build_dam_sample_app.bat
```
* Push app with **QFLOG**
```shell
SDK> python.exe .\QFLOG\src\QFLOGPackage\QFLOG.py -p <COMPORT> PUSH -f <FilePath>
```
* Enable log view
```shell
SDK> python.exe .\QFLOG\src\QFLOGPackage\QFLOG.py -p <COMPORT> VIEW_LOGS
```

## 2. Source Code Merge
### 2.1 The initialization processing logic of cOAP module in DSS callback function
#### File path
> Sample_Applications\io9205_track\src\demo\io9205_net.c
#### Function name
> **t95_dss_cb**
#### Logic implementation：
```C++
static void 
t95_dss_cb( qapi_DSS_Hndl_t         hndl,          /* Handle for which this event is associated */
        void                   *user_data,     /* Application-provided user data */
        qapi_DSS_Net_Evt_t      evt,           /* Event identifier */
        qapi_DSS_Evt_Payload_t *payload_ptr)   /* Associated event information */
{
	// ...
	// Call t95_coap_init in "QAPI_DSS_EVT_NET_IS_CONN_E"
    if (evt == QAPI_DSS_EVT_NET_IS_CONN_E)
    {
		// ...
		ret = t95_coap_init();
		if (ret != 0)
		{
			LOG_ERROR("*****t95_coap_init failed!***** ret = %d", ret);
		}

	}
	// ...
}
```

### 2.2 Button message business logic in business cycle
#### File path : 
> Sample_Applications\io9205_track\src\demo\track_demo_module.c
#### Function name：
> **dam_app_routine**
#### Logic implementation：
```C++
void dam_app_routine(void)
{
	// ...
	while (1)
	{
		// ...
		//  Call upload_dev_info at "while (1) "
        ret = upload_dev_info();
		if (ret == 0)
		{
			LOG_INFO("upload_dev_info succeed\n");
			qapi_Timer_Sleep(3, QAPI_TIMER_UNIT_SEC, true);
		}
		else
		{
			LOG_INFO("upload_dev_info failed\n");
			qapi_Timer_Sleep(3, QAPI_TIMER_UNIT_SEC, true);
		}
		LOG_INFO("upload_dev_info end\n");
		// ...
	}	
	// ...	
}
```
### 2.3 Realization of message reporting function in network module
#### File path : 
> Sample_Applications\io9205_track\src\demo\io9205_net.c
#### Function name：
> **upload_dev_info**
#### Logic implementation：
```C++
int upload_dev_info(void)
{
    LOG_INFO("=====upload_dev_info start.=====\n");
    LOG_INFO("======================= upload_dev_info sos msg =======================\n");
    #if 1
    LOG_INFO("upload_dev_info alarm msg\n");
    if (QAPI_ERROR == app_coap_send_tracker_alarm_message_func())
    {
        LOG_INFO("upload_dev_info alarm msg error\n");
        return -1;
    }
    LOG_INFO("upload_dev_info loc msg\n");
    if (QAPI_ERROR == app_coap_send_tracker_loc_message_func())
    {
        LOG_INFO("upload_dev_info loc msg error\n");
        return -1;
    }
    #endif
    LOG_INFO("======================= upload_dev_info sos msg end! =======================\n");
    return 0;
}
```


### 2.4  Add cOAP module to the project
* 1) Import io9205_coap_control.h io9205_coap_control.c File path.
* 2) Add a reference to io9205_coap_control.h in io9205_net.h and track_demo_module.h

## 3 Joint Debugging
Flash fireware for T95, turn on TurboX T95, start joint debugging:

* 1. Wait 1-2 minutes for SIM card network registration after powered up
* 2. Click the button2 on T95
* 3. Log in to the website from server
* 4. Check the reported data based on the <user guide>

