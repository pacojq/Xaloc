﻿using System;
using System.Runtime.CompilerServices;

namespace Xaloc
{
    public class TagComponent : Component
    {
        public string Tag
        {
            get => GetTag_Native(Entity.ID);
            set => SetTag_Native(Entity.ID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern string GetTag_Native(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetTag_Native(ulong entityID, string tag);

    }
}
