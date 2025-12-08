using System;
using System.Runtime.CompilerServices;

namespace Xaloc
{
    public class Log
    {
        public static void Fatal(string message)
        {
            Fatal_Native(message);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Fatal_Native(string message);

        public static void Error(string message)
        {
            Error_Native(message);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Error_Native(string message);

        public static void Warn(string message)
        {
            Warn_Native(message);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Warn_Native(string message);

        public static void Info(string message)
        {
            Info_Native(message);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Info_Native(string message);

        public static void Trace(string message)
        {
            Trace_Native(message);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Trace_Native(string message);
    }
}
