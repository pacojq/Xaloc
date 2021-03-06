﻿using System;
using Xaloc;

namespace SandboxCs
{
    class PlayerEntity : Entity
    {
        public float Speed = 5.0f;

        public void OnCreate()
        {
            Log.Info("Hello world!");

            TagComponent tag = GetComponent<TagComponent>();
            tag.Tag = "Tag from CSharp!";

            Log.Info($"We have a new tag: { GetComponent<TagComponent>().Tag}");
        }

        public void OnUpdate(float ts)
        {
            //Log.Warn("Hi there!");

            Matrix4 transform = GetTransform();
            Vector3 translation = transform.Translation;

            float speed = Speed * ts;

            if (Input.IsKeyPressed(KeyCode.Up))
                translation.Y += speed;
            else if (Input.IsKeyPressed(KeyCode.Down))
                translation.Y -= speed;
            if (Input.IsKeyPressed(KeyCode.Right))
                translation.X += speed;
            else if (Input.IsKeyPressed(KeyCode.Left))
                translation.X -= speed;

            transform.Translation = translation;
            SetTransform(transform);
        }
    }
}
