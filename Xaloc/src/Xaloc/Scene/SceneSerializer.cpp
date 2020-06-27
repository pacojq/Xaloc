#include "xapch.h"
#include "SceneSerializer.h"

#include "Scene.h"
#include "Components.h"

namespace Xaloc {

	Ref<Scene> SceneSerializer::Deserialize(pugi::xml_document& doc, pugi::xml_parse_result& result)
	{
		Ref<Scene> scene = CreateRef<Scene>("Sandbox Scene");

		return scene;
	}

	pugi::xml_document SceneSerializer::Serialize(const Ref<Scene>& scene)
	{
        pugi::xml_document doc;
        if (!doc.load_string("<!-- Xaloc Engine. Serialized Scene -->", pugi::parse_default | pugi::parse_comments))
        {
            XA_CORE_ASSERT(false, "Something went wrong!");
        }

        pugi::xml_node root = doc.append_child("scene");
        auto rootId = root.append_attribute("id");
        rootId.set_value(scene->m_SceneID);
        
        pugi::xml_node sceneName = root.append_child("name");
        sceneName.append_child(pugi::node_pcdata).set_value(scene->m_Name.c_str());
        

        pugi::xml_node entities = root.append_child("entities");
        

        auto view = scene->m_Registry.view<IdComponent>();
        for (auto entity : view)
        {
            auto& idComp = view.get<IdComponent>(entity);
            //auto ent = Entity(entity, scene.get());
            SerializeEntity(doc, entities, Entity(entity, scene.get()), idComp.ID, scene);
        }

        return doc;
	}

    void SceneSerializer::SerializeEntity(pugi::xml_document& doc, pugi::xml_node& entitiesRoot,
            Entity entity, uint32_t id, const Ref<Scene>& scene)
    {
        pugi::xml_node node = entitiesRoot.append_child("entity");

        auto attrId = node.append_attribute("id");
        attrId.set_value(id);

        if (entity.HasComponent<TagComponent>())
        {
            auto tag = entity.GetComponent<TagComponent>();
            auto tagNode = node.append_child("TagComponent");
            tagNode.append_attribute("tag").set_value(tag.Tag.c_str());
        }

        if (entity.HasComponent<TransformComponent>())
        {
            auto transform = entity.GetComponent<TransformComponent>();
            auto transformNode = node.append_child("TransformComponent");

            auto [translation, rotation, scale] = GetTransformDecomposition(transform);
            char buff[64];
            
            snprintf(buff, sizeof(buff), "[%f, %f, %f]", translation.r, translation.g, translation.b);
            std::string strPosition = buff;
            snprintf(buff, sizeof(buff), "[%f, %f, %f, %f]", rotation.w, rotation.x, rotation.y, rotation.z);
            std::string strRotation = buff;
            snprintf(buff, sizeof(buff), "[%f, %f, %f]", scale.r, scale.g, scale.b);
            std::string strScale = buff;

            transformNode.append_attribute("position").set_value(strPosition.c_str());
            transformNode.append_attribute("rotation").set_value(strRotation.c_str());
            transformNode.append_attribute("scale").set_value(strScale.c_str());
        }

        if (entity.HasComponent<BehaviourComponent>())
        {
            auto behaviour = entity.GetComponent<BehaviourComponent>();
            auto behaviourNode = node.append_child("BehaviourComponent");
            behaviourNode.append_attribute("name").set_value(behaviour.ModuleName.c_str());
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            auto renderer = entity.GetComponent<SpriteRendererComponent>();
            auto rendererNode = node.append_child("SpriteRendererComponent");

            auto subTex = renderer.SubTexture;
            auto tex = subTex->GetTexture();
            
            char buff[64];
            snprintf(buff, sizeof(buff), "[%f, %f]", subTex->GetTexCoords()->x, subTex->GetTexCoords()->y);
            std::string strTexcoords = buff;

            rendererNode.append_attribute("texcoords").set_value(strTexcoords.c_str());
            rendererNode.append_attribute("width").set_value(subTex->GetWidth());
            rendererNode.append_attribute("height").set_value(subTex->GetHeight());
            rendererNode.append_attribute("path").set_value(tex->GetPath().c_str());
        }
    }

}