#include "xapch.h"
#include "SceneSerializer.h"

#include "Scene.h"
#include "Components.h"

#include "Xaloc/Core/Assets/AssetManager.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Xaloc {

    static std::vector<float> ParseVector(const std::string& str)
    {
        std::vector<float> output;
        std::string s = std::string(str);
        char separator = ',';

        s.erase(0, 1); // Remove first "["
        s.erase(str.length() - 1, 1); // Remove last "]"

        std::string::size_type prev_pos = 0, pos = 0;

        while ((pos = s.find(separator, pos)) != std::string::npos)
        {
            std::string substring(s.substr(prev_pos, pos - prev_pos));

            output.push_back(std::stof(substring));

            prev_pos = ++pos;
        }

        output.push_back(std::stof(s.substr(prev_pos, pos - prev_pos))); // Last word

        return output;
    }

    static uint64_t ParseUint64(const std::string& str)
    {
        errno = 0;
        unsigned long long result = strtoull(str.c_str(), NULL, 16);

        if (errno == EINVAL)
        {
            XA_CORE_ASSERT(false, "Not a valid number!");
            return 0;
        }
        else if (errno == ERANGE)
        {
            XA_CORE_ASSERT(false, "Does not fit in an unsigned long long!");
            return 0;
        }

        return result;
    }

    static std::string Uint64ToString(uint64_t n)
    {
        char buff[16 + 1];
        snprintf(buff, sizeof(buff), "%llx", n);
        std::string result = buff;
        return result;
    }

    




	Ref<Scene> SceneSerializer::Deserialize(pugi::xml_document& doc)
	{
        pugi::xml_node root = doc.child("scene");

        std::string strId = root.attribute("uuid").value();
        uint64_t id = ParseUint64(strId);
        std::string name = root.child("name").child_value();

        XA_CORE_TRACE("Loading scene. Id = {0} ({1}). Name = '{2}'", id, strId, name);
        
		Ref<Scene> scene = CreateRef<Scene>(name, id);


        pugi::xml_node entitiesRoot = root.child("entities");

        //for (pugi::xml_node entity = entitiesRoot.child("entity"); entity; entity = entity.next_sibling("entity"))
        //{
        //    DeserializeEntity(entity, scene);
        //}

        for (pugi::xml_node entity = entitiesRoot.last_child(); entity; entity = entity.previous_sibling())
        {
            DeserializeEntity(entity, scene);
        }

		return scene;
	}


    void SceneSerializer::DeserializeEntity(pugi::xml_node& entityNode, const Ref<Scene>& scene)
    {
        std::string strId = entityNode.attribute("uuid").value();
        uint64_t id = ParseUint64(strId);
        XA_CORE_TRACE("    Loading entity. Id = {}  ({})", id, strId);

        auto entity = scene->CreateEntity("New Entity", id);

        
        pugi::xml_node tagNode = entityNode.child("TagComponent");
        if (tagNode)
        {
            std::string tag = tagNode.attribute("tag").value();
            entity.GetComponent<TagComponent>().Tag = tag;

            XA_CORE_TRACE("        TagComponent: {}", tag);
        }

        pugi::xml_node transformNode = entityNode.child("TransformComponent");
        if (transformNode)
        {
            std::string strPosition = transformNode.attribute("position").value();
            auto vecPosition = ParseVector(strPosition);
            glm::vec3 position = { vecPosition[0], vecPosition[1], vecPosition[2] };

            std::string strRotation = transformNode.attribute("rotation").value();
            auto vecRotation = ParseVector(strRotation);
            glm::quat rotation = { vecRotation[0], vecRotation[1], vecRotation[2], vecRotation[3] };

            std::string strScale = transformNode.attribute("scale").value();
            auto vecScale = ParseVector(strScale);
            glm::vec3 scale = { vecScale[0], vecScale[1], vecScale[2] };

            
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                * glm::mat4_cast( rotation )
                * glm::scale(glm::mat4(1.0f), scale);

            auto [oPosition, oRotation, oScale] = GetTransformDecomposition(transform);

            XA_ASSERT(oPosition == position, "Failed to deserialize position");
            XA_ASSERT(oRotation == rotation, "Failed to deserialize rotation");
            XA_ASSERT(oScale == scale, "Failed to deserialize scale");

            entity.GetComponent<TransformComponent>().Transform = transform;

            XA_CORE_TRACE("        TransformComponent: position = {0}    rotation = {1}    scale = {2}",
                strPosition, strRotation, strScale);
        }

        pugi::xml_node behaviourNode = entityNode.child("BehaviourComponent");
        if (behaviourNode)
        {
            std::string name = behaviourNode.attribute("name").value();
            entity.AddComponent<BehaviourComponent>(name);

            XA_CORE_TRACE("        BehaviourComponent: {}", name);
        }

        pugi::xml_node rendererNode = entityNode.child("SpriteRendererComponent");
        if (rendererNode)
        {
            std::string strTexcoords = rendererNode.attribute("texcoords").value();
            auto texcoords = ParseVector(strTexcoords);

            uint32_t width = rendererNode.attribute("width").as_uint();
            uint32_t height = rendererNode.attribute("height").as_uint();

            std::string assetID = rendererNode.attribute("assetID").value();
            
            Ref<Texture2D> tilemap = AssetManager::GetTexture(assetID);
            Ref<SubTexture2D> subTex = SubTexture2D::CreateFromAbsCoords(tilemap, 
                { texcoords[0] * tilemap->GetWidth(), texcoords[1] * tilemap->GetHeight() }, { width, height });

            entity.AddComponent<SpriteRendererComponent>(subTex);

            XA_CORE_TRACE("        SpriteRendererComponent: texcoords = {0}    width = {1}    height = {2}    assetID = {3}",
                strTexcoords, width, height, assetID);
        }
    }





	pugi::xml_document SceneSerializer::Serialize(const Ref<Scene>& scene)
	{
        pugi::xml_document doc;
        pugi::xml_parse_result res = doc.load_string("<!-- Xaloc Engine. Serialized Scene -->", pugi::parse_default | pugi::parse_comments);

    	if (!res && res.status != pugi::status_no_document_element)
        {
            XA_CORE_ERROR("Parsing status: {}", res.status);
            XA_CORE_ERROR("Parsing error: {}", res.description());
            XA_CORE_ASSERT(false, "Something went wrong!");
        }

        pugi::xml_node root = doc.append_child("scene");
    	
        auto rootId = root.append_attribute("uuid");
        rootId.set_value(Uint64ToString(scene->m_SceneID).c_str());
        
        pugi::xml_node sceneName = root.append_child("name");
        sceneName.append_child(pugi::node_pcdata).set_value(scene->m_Name.c_str());
        

        pugi::xml_node entities = root.append_child("entities");
        
        // Only serialize entities with ID
        auto view = scene->m_Registry.view<IdComponent>();
        for (auto entity : view)
        {
            auto& idComp = view.get<IdComponent>(entity);
            SerializeEntity(doc, entities, Entity(entity, scene.get()), idComp.ID, scene);
        }

        return doc;
	}

    void SceneSerializer::SerializeEntity(pugi::xml_document& doc, pugi::xml_node& entitiesRoot,
            Entity entity, UUID id, const Ref<Scene>& scene)
    {
        pugi::xml_node node = entitiesRoot.append_child("entity");

        auto attrId = node.append_attribute("uuid");
        attrId.set_value(Uint64ToString(id).c_str());

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
            rendererNode.append_attribute("assetID").set_value(tex->AssetID().c_str());
        }
    }

}