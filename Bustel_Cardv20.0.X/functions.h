//Functions used
void init();
void DelayDs(int);
void WriteSPI(unsigned char);
unsigned char ReadSPI(void);
void TransiverInitReceiving(void); 
void TransiverInitTransmitter(void);
void TransiverInit(void);
void WriteControlRegTransiver(unsigned char , unsigned char);
void Blink(int);
void TransiverSleep(void);
void TransiverRX(void);
void TransiverTX(void);
void TransiverStandBy(void);
void SendPacket(char);
void SetRFMode(char);
unsigned char RegisterRead(unsigned char);
void RegisterSet(unsigned char address, unsigned char value);
unsigned char ReadFIFO(void);
void WriteFIFO(unsigned char);
unsigned char AnalogValue(unsigned char);
void DarknessCheck(void);
unsigned char OperationMode(void);
void LightWithSensController(void);
unsigned char timerFunction(int intTimerNumber, int intSetSeconds );
void Mode_0();
void Mode_1();
void Mode_2();
void Mode_3();
void Mode_4();
void Mode_5();
void Mode_6();
void Mode_7();
void TransiverToReceive();
void TransiverReadFIFO();
void ReceivedDataHandler();
void TransmittPacket(char,char);
char FindChannel(void);