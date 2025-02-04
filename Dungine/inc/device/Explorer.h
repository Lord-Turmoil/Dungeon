/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Explorer.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 11, 2022                             *
 *                                                                            *
 *                    Last Update : July 27, 2022                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Explorer provides game objects access to resources.                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _EXPLORER_H_
#define _EXPLORER_H_

#include <easyx.h>
#include <string>
#include <unordered_map>

#include "../common/Common.h"
#include "../template/Singleton.h"
#include "../utility/xml.h"

/********************************************************************
** Forward declaration of resource types.
*/
class AbstractSound;
class MotionSet;

/********************************************************************
** Emm... The resource system is not well designed. :(
** The resource is pre-loaded as reference, and will not load the
** actual resource until required.
** Since some resources are generated internal, they do not need
** xml info.
*/

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Resource Region
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
enum class ResourceType
{
    RES_AUDIO,
    RES_IMAGE,
    RES_MOTION,

    RES_NUM
};

class AbstractResource
{
public:
    AbstractResource(ResourceType type, const std::string& id)
        : m_id(id), m_type(type), m_xmlNode(nullptr), m_refCount(0)
    {
    }

    virtual ~AbstractResource()
    {
    }

    ResourceType Type() const
    {
        return m_type;
    }

    const std::string& GetIdentifier() const
    {
        return m_id;
    }

    /*
    ** External resource will be kept in a xml file. If internal
    ** resource is needed, such as image, the resource must be
    ** got from new, so it can be correctly deleted.
    */
    void AssignExternal(XMLElement* node);
    virtual void AssignInternal(void* pResource) = 0;

    virtual bool Load() = 0;

    void Retain();  // Reference count ++
    void Release(); // Reference count --

protected:
    virtual void _AutoRelease() = 0;

    std::string m_id;
    ResourceType m_type;
    XMLElement* m_xmlNode;
    int m_refCount;
};

class AudioResource : public AbstractResource
{
public:
    AudioResource(const std::string& id) : AbstractResource(ResourceType::RES_AUDIO, id), m_pResource(nullptr)
    {
    }

    ~AudioResource() override;

    AbstractSound* GetResource();

    void AssignInternal(void* pResource) override;

    bool Load() override;

protected:
    void _AutoRelease() override;

private:
    AbstractSound* _LoadSound(XMLElement* node);

    AbstractSound* m_pResource;
};

class ImageResource : public AbstractResource
{
public:
    ImageResource(const std::string& id) : AbstractResource(ResourceType::RES_IMAGE, id), m_pResource(nullptr)
    {
    }

    ~ImageResource() override;

    IMAGE* GetResource();

    void AssignInternal(void* pResource) override;

    bool Load() override;

protected:
    void _AutoRelease() override;

private:
    IMAGE* _LoadImage(XMLElement* node);

    IMAGE* m_pResource;
};

class MotionResource : public AbstractResource
{
public:
    MotionResource(const std::string& id) : AbstractResource(ResourceType::RES_MOTION, id), m_pResource(nullptr)
    {
    }

    ~MotionResource() override;

    MotionSet* GetResource();

    void AssignInternal(void* pResource) override;

    bool Load() override;

protected:
    void _AutoRelease() override;

private:
    MotionSet* _LoadMotion(XMLElement* node);

    MotionSet* m_pResource;
};

AbstractResource* LoadResourceItem(XMLElement* node);
AbstractResource* MakeResourceItem(ResourceType type, const std::string& id, void* pResource);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Explorer Region
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class Explorer : public Singleton<Explorer>
{
    friend class Singleton<Explorer>;

public:
    bool Load(const char* filename);
    void UnLoad();

    AbstractResource* GetResource(const std::string& id);

    /*
    ** Load or release existing item.
    */
    bool LoadResource(const std::string& id);
    bool ReleaseResource(const std::string& id);

    bool AssignInternalResource(ResourceType type, const std::string& id, void* pResource);

private:
    Explorer() : m_isLoaded(false)
    {
    }

    ~Explorer() override;

    bool _AddResource(AbstractResource* resource);
    void _Load();
    void _UnLoad();

    XMLFile m_config; // The xml file will be kept internal.
    std::unordered_map<std::string, AbstractResource*> m_pool;
    bool m_isLoaded;
};

template <typename ResourceType> ResourceType* LoadResource(const std::string& id)
{
    ResourceType* res = dynamic_cast<ResourceType*>(Explorer::GetInstance()->GetResource(id));

    if (!res)
    {
        LOG_ERROR(INVALID_RESOURCE_ID, id.c_str());
        return nullptr;
    }

    res->Retain();

    return res;
}

#endif
