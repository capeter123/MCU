#----*-coding:utf-8-*----

from ctypes import *
import time
import datetime
import math
import sched

class CanObject():
    CanId = 0x0
    CanDlc = 0x0
    CanFlag = 0x0
    CanTime = 0x0
    CanMsg = []
    
class CanHistoryListParser():
    canObjects = []
    def __init__(self, filename):
        hFile = open(filename)
        for line in hFile:
            obj = CanObject()
            fields = line.split()
            obj.CanId = int(fields[1])
            obj.CanDlc = int(fields[2])
            obj.CanMsg = map(int, fields[3:])
            self.canObjects.append(obj)

class CANDriver():
    ## CAN internal properties.
    __channel = 0
    __handle = -1
    __canlib = None
    __init = False
    __scheduler = None
    ## CAN Status defines.
    canOk = 0
    canErr_NOMSG = 2
    ## CAN Frame Type defines.
    canMSG_STD = 0x0002
    canMSG_ERROR_FRAME = 0x0020
    ## CAN Buffer
    __canMsg = (c_ubyte*8)(0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0)
    __pCanMsg = None
    def __init__(self):
        self.__scheduler = sched.scheduler(time.time, time.sleep)
        self.__pCanMsg = cast(self.__canMsg, POINTER(c_ubyte))
        self.__canlib = cdll.LoadLibrary('canlib32.dll')
        self.__init = self.__canInit()
    
    def __canInit(self):
        ret = False
        canStatus = self.__canlib.canInitializeLibrary()
        if(self.canOk == canStatus):
            print "Initialize CANlib successfully!"
            chNum = c_int(0)
            canStatus = self.__canlib.canGetNumberOfChannels(byref(chNum))
            print "Current avaliable channels = %d"%(chNum.value)
            chName = c_char_p("/0"*50)
            isFound = False
            for ch in range(0,chNum.value):
                self.__canlib.canGetChannelData(ch, 13, chName, len(chName.value))
                if(True == chName.value.startswith("Kvaser Leaf Light")):
                    isFound = True
                    break;

            if(True == isFound):
                self.__channel = ch
                print "Ch[%d]:%s"%(ch, chName.value)
                ret = True

            del(chName)
        else:
            print "Initialize CANLib failed!"

        return ret
    
    def canBusOn(self):
        ret = False
        if not self.__init:
            return False
        
        #S1. open CAN channel to get channel handle
        self.__handle = self.__canlib.canOpenChannel(self.__channel, 0x100)
        if(self.__handle >= 0):
            #S2. set CAN bus paramters.
            canState = self.__canlib.canSetBusParams(self.__handle, 500000, 1, 1, 3, 1, 1)
            #S3. open CAN channel
            if(self.canOk == canState):
                canState = self.__canlib.canBusOn(self.__handle)
                if(self.canOk == canState):
                    ret = True

        return ret

    def canBusOff(self):
        if not self.__init:
            return False
        
        canState = self.__canlib.canBusOff(self.__handle)
        return (canState == self.canOk)

    def canSend(self, canId, msg, flag = 0x0002):
        if not self.__init:
            return False

        for i in range(0,len(msg)):
            self.__canMsg[i] = msg[i]
            
        canState = self.__canlib.canWrite(self.__handle, canId&0x7FF, self.__pCanMsg, len(msg), flag)
        return (self.canOk==canState)

    def canRead(self, obj, timeout=500):
        if not self.__init:
            return False
        
        canId = c_long(0)
        canDlc = c_uint(0)
        canFlag = c_uint(0)
        canTime = c_ulong(0)
        canStatus = self.__canlib.canReadWait(self.__handle,
                                          byref(canId),
                                          self.__pCanMsg,
                                          byref(canDlc),
                                          byref(canFlag),
                                          byref(canTime),
                                          timeout)

        if(canStatus == self.canOk):
            obj.CanId = canId.value
            obj.CanMsg = [b for b in self.__canMsg]
            obj.CanDlc = canDlc.value
            obj.CanTime = canTime.value
            obj.CanFlag = canFlag.value
        else:
            print "No message on the bus!"
            
        return canStatus == self.canOk

    def __canDummy(self):
        pass
    
    def canWait(self, sec):
        #time.sleep(sec)
        self.__scheduler.enter(sec, 1, self.__canDummy, ())
        self.__scheduler.run()
        
    def __del__(self):
        del(self.__pCanMsg)
        del(self.__canMsg)
        self.canBusOff()
        self.__canlib.canClose(self.__handle)

class CANTester():
    __cycleTimes = 1
    __interval = 0.005 # Second.
    def __init__(self):
        print "Initialized CanTester..."
        self.__canHisParser = CanHistoryListParser("./531_TCU_Kvaser_BCM.hst")
        self.__canDriver = CANDriver()
        self.__canDriver.canBusOn()
        print "Initialized CanTester Finished!"

    def run(self):
        canRxObj = CanObject()
        canTxObj = CanObject()
        canTxObj.CanId = 0x746
        canTxObj.CanMsg = (0x03, 0x22, 0xEF, 0x82)
        canTxObj.CanDlc = 0x08
        msgBuf = []   
        canObjList = self.__canHisParser.canObjects # backup object list.
        canObj_filterout = filter(lambda obj:obj.CanId==0x60D, canObjList)   
        print "Start Test... [%s]"%(self.print_time())
        # S0. Send 0x14 to clear all DTC and Counters
        self.__canDriver.canSend(0x746, (0x04, 0x14, 0xFF, 0xFF, 0xFF))
        self.__canDriver.canRead(canRxObj) # dummy read
        self.__canDriver.canWait(0.1)
        #self.__canDriver.canSend(0x551, (0x42, 0x00, 0x00, 0xFF, 0x00, 0x80, 0x00, 0xFF))
        #self.__receiveUUDT(canTxObj, msgBuf)
        # S1. Send TBox Ecu Frames...
        for i in range(self.__cycleTimes):
            for obj in canObjList:
                if(self.__canDriver.canSend(obj.CanId, obj.CanMsg)):
                    self.__canDriver.canWait(self.__interval)
                    #print "Send Msg [CanId=0x%X, Dlc=%d] successfully"%(obj.CanId, obj.CanDlc)
        # S2. Continue sending TBox Ecu Frames except for 0x180, keep it about 1700ms.
        canObjList.remove(canObj_filterout[0])
        elapsedTime = 0.0
        startTime = datetime.datetime.now()
        while(elapsedTime < 1.5):
            for obj in canObjList:
                if(self.__canDriver.canSend(obj.CanId, obj.CanMsg)):
                    self.__canDriver.canWait(self.__interval)

            elapsedTime = (datetime.datetime.now() - startTime).total_seconds()
          
        # S3. Send UDS Frames to read DIDEF82.
        msgBuf=[]
        self.__receiveUUDT(canTxObj, msgBuf)

        # S4. Send All Ecu Frames.
        canObjList.append(canObj_filterout[0])
        for i in range(10):
            for obj in canObjList:
               if(self.__canDriver.canSend(obj.CanId, obj.CanMsg)):
                    self.__canDriver.canWait(self.__interval)

        # S5. Read EF82
        msgBuf = [] # clear buffer before read.
        self.__receiveUUDT(canTxObj, msgBuf)

        print ">>Finished Test! [%s]"%(self.print_time())
        #self.__canDriver.canSend(0x746, (0x02, 0x3E, 0x00))

    def print_time(self):
        return datetime.datetime.now().strftime("%H:%M:%S.%f")

    def __receiveUUDT(self, reqCmdObj, msgBuf):
        ret = self.__canDriver.canSend(reqCmdObj.CanId, reqCmdObj.CanMsg)
        if ret:
            canObj = CanObject()
            msgLen = 0
            # S1. Receive the first frame or single frame.
            ret = self.__canDriver.canRead(canObj)
            if ret:
                if(canObj.CanMsg[0]&0xF0 == 0x10): # First Frame
                    msgLen = ((canObj.CanMsg[0]&0x0F)<<8) | canObj.CanMsg[1]
                    snNum = msgLen/7
                    sn = 1
                    remainBytes = (msgLen-6)%7
                    map(msgBuf.append, canObj.CanMsg[2:]) # load the first frame bytes.
                    msgFC = (0x30,0x01,0x0,0x0,0x0,0x0,0x0,0x0)
                    while(ret == True and sn <= snNum):
                        # Send Flow Control Frame
                        ret = self.__canDriver.canSend(reqCmdObj.CanId, msgFC)
                        ret = self.__canDriver.canRead(canObj)
                        if(canObj.CanMsg[0]&0x0F != sn&0x0F):
                            ret = False
                            break

                        if(sn == snNum and remainBytes!=0): # last frame
                            map(msgBuf.append, canObj.CanMsg[1:remainBytes+1])
                        else:
                            map(msgBuf.append, canObj.CanMsg[1:])

                        sn = sn + 1
                            
                    if ret:
                        print "__receiveUUDT successfully"
                    else:
                        print "__receiveUUDT Error at sn=%d"%sn
                        
                elif(canObj.CanMsg[0]&0xF0 == 0x0): # Single Frame
                    msgLen = canObj.CanMsg[0]&0xFF
                    canObj.CanMsg = canObj.CanMsg[1:msgLen+1]
                    map(msgBuf.append, canObj.CanMsg)
                else:
                    print "Error Response Frame!"
    
if __name__ == "__main__":
    tester = CANTester()
    tester.run()
