using System;
using System.Runtime.CompilerServices;

namespace Xaloc
{
    public class TagComponent : Component
    {
        public string Tag
        {
            get => GetTag_Native(Entity.SceneID, Entity.EntityID);
            set => SetTag_Native(Entity.SceneID, Entity.EntityID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern string GetTag_Native(uint sceneID, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetTag_Native(uint sceneID, uint entityID, string tag);

    }
}
