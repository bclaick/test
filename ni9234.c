#include <stdio.h>
#include <stdlib.h>
#include <NIDAQmx.h>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

int main(void) {
    int32       error = 0;
    TaskHandle  taskHandle = 0;
    char        errBuff[2048] = {'\0'};
    float64     data[2048];
    int32       read;

    // 1. Create a DAQmx Task
    DAQmxErrChk (DAQmxCreateTask("AnalogInputTask", &taskHandle));

    // 2. Configure an Analog Input Voltage Channel on your simulated NI9234
    // Change "NI9234/ai0" to your specific device name if needed (e.g., "cDAQ1Mod1/ai0")
    DAQmxErrChk (DAQmxCreateAIVoltageChan(taskHandle, "NI9234/ai0", "", DAQmx_Val_Cfg_Default, -5.0, 5.0, DAQmx_Val_Volts, NULL));

    // 3. Configure the Sampling Clock (Continuous Sampling at 2.048 kHz)
    DAQmxErrChk (DAQmxCfgSampClkTiming(taskHandle, "", 20480.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 2048));

    // 4. Start the Task
    DAQmxErrChk (DAQmxStartTask(taskHandle));

    printf("Successfully communicating with simulated device. Reading data...\n");

    // 5. Read 1000 samples from the channel
    DAQmxErrChk (DAQmxReadAnalogF64(taskHandle, 2048, 10.0, DAQmx_Val_GroupByChannel, data, 2048, &read, NULL));

    printf("Acquisition complete. Successfully read %d samples.\n", (int)read);
    printf("First sample value: %f V\n", data[0]);

Error:
    if( DAQmxFailed(error) ) {
        DAQmxGetExtendedErrorInfo(errBuff, 2048);
        printf("DAQmx Error: %s\n", errBuff);
    }

    // 6. Clean up and release resource handles
    if( taskHandle != 0 ) {
        DAQmxStopTask(taskHandle);
        DAQmxClearTask(taskHandle);
    }
    return 0;
}

