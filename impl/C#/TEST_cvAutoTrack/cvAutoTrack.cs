﻿
using System.Runtime.InteropServices;

namespace cvAutoTrack
{
    // 导入 cvAutoTrack.dll
    public class cvAutoTrack
    {
        //[DllImport("cvAutoTrack.dll", EntryPoint = "verison")]
        //public static extern bool verison(ref byte[] versionBuff);
        
        [DllImport("cvAutoTrack.dll", EntryPoint = "GetStar")]
        public static extern bool GetStar(ref double x, ref double y, ref bool isEnd);




        [DllImport("cvAutoTrack.dll", EntryPoint = "GetStarJson", CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetStarJson(ref byte[] jsonBuffer);

        [DllImport("cvAutoTrack.dll")]
        public static extern bool verison([MarshalAs(UnmanagedType.LPStr)]ref char[] versionBuff);

    }

}