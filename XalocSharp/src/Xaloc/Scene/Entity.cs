﻿using System;
using System.Runtime.CompilerServices;

namespace Xaloc
{
    public class Entity
    {
        public ulong ID { get; private set; }

        ~Entity()
        {
            Console.WriteLine("Destroyed Entity {0}", ID);
        }

        public T CreateComponent<T>() where T : Component, new()
        {
            CreateComponent_Native(ID, typeof(T));
            T component = new T();
            component.Entity = this;
            return component;
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void CreateComponent_Native(ulong entityID, Type type);


        public bool HasComponent<T>() where T : Component, new()
        {
            return HasComponent_Native(ID, typeof(T));
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool HasComponent_Native(ulong entityID, Type type);


        public T GetComponent<T>() where T : Component, new()
        {
            if (HasComponent<T>())
            {
                T component = new T();
                component.Entity = this;
                return component;
            }
            return null;
        }

        public Matrix4 GetTransform()
        {
            Matrix4 mat4Instance;
            GetTransform_Native(ID, out mat4Instance);
            return mat4Instance;
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetTransform_Native(ulong entityID, out Matrix4 matrix);


        public void SetTransform(Matrix4 transform)
        {
            SetTransform_Native(ID, ref transform);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTransform_Native(ulong entityID, ref Matrix4 matrix);

    }
}
