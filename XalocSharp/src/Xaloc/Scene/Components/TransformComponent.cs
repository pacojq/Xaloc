using System;
using System.Runtime.CompilerServices;

namespace Xaloc
{
    public class TransformComponent : Component
    {
        public Matrix4 Transform
        {
            get
            {
                Matrix4 result;
                GetTransform_Native(Entity.SceneID, Entity.EntityID, out result);
                return result;
            }
            set
            {
                SetTransform_Native(Entity.SceneID, Entity.EntityID, ref value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void GetTransform_Native(uint sceneID, uint entityID, out Matrix4 result);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetTransform_Native(uint sceneID, uint entityID, ref Matrix4 result);

    }
}
