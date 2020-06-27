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
    }

}