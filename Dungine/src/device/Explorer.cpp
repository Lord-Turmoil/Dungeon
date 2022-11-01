/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Explorer.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 11, 2022                             *
 *                                                                            *
 *                    Last Update : July 27, 2022                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Explorer provides game objects access to resources.                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/device/Explorer.h"
#include "../../inc/device/Sound.h"

#include "../../inc/game/Animation.h"

#include "../../inc/utility/Auxilliary.h"
#include "../../inc/utility/Parser.h"
#include "../../inc/utility/Straw.h"


 /******************************************************************************
  * AbstractResource::AssignExternal -- Assigned external resource.            *
  *                                                                            *
  *    Assigned external resource reference. The resource will not be finally  *
  *    loaded to memory until required or forced.                              *
  *                                                                            *
  * INPUT:   node                                                              *
  *                                                                            *
  * OUTPUT:  none                                                              *
  *                                                                            *
  * WARNINGS:  none                                                            *
  *                                                                            *
  * HISTORY:                                                                   *
  *   2022/06/13 Tony : Created.                                               *
  *============================================================================*/
void AbstractResource::AssignExternal(XMLElement* node)
{
	m_xmlNode = node;
}


/******************************************************************************
 * AbstractResource::Retain -- Increase the reference count by one.           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
void AbstractResource:: Retain()
{
	m_refCount++;
}


/******************************************************************************
 * AbstractResource::Release -- Decrease the reference count by one.          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
void AbstractResource::Release()
{
	if (m_refCount > 0)
	{
		m_refCount--;
		if (m_refCount == 0)
			_AutoRelease();
	}
	else
		LOG_WARNING(RELEASE_UNREFERENCED, m_id.c_str());
}


/******************************************************************************
 * AudioResource::~AudioResource -- Destructor of the object.                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/13 Tony : Created.                                               *
 *============================================================================*/
AudioResource::~AudioResource()
{
	if (m_pResource)
		_AutoRelease();
}


/******************************************************************************
 * AudioResource::GetResource -- Get resource.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return sound resource.                                            *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/13 Tony : Created.                                               *
 *============================================================================*/
AbstractSound* AudioResource::GetResource()
{
	if (!m_pResource)
	{
		Load();
		if (!m_pResource)	// Load failed.
		{
			LOG_ERROR(FAILED_TO_LOAD_RESOURCE, m_id.c_str());
			return nullptr;
		}
	}

	return m_pResource;
}


/******************************************************************************
 * AudioResource::AssignInternal -- Assign internal resource.                 *
 *                                                                            *
 *    Since it do not have the ability to create internal sound, this function*
 *    is empty.                                                               *
 *                                                                            *
 * INPUT:   pResource                                                         *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/13 Tony : Created.                                               *
 *============================================================================*/
void AudioResource::AssignInternal(void* pResource)
{
}


/******************************************************************************
 * AudioResource::Load -- Load audio resource.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/13 Tony : Created.                                               *
 *============================================================================*/
bool AudioResource::Load()
{
	if (m_pResource)
		return true;

	if (!m_xmlNode)
	{
		LOG_ERROR(MISSING_REFERENCE, m_id.c_str());
		return false;
	}

	m_pResource = _LoadSound(m_xmlNode);

#ifdef DGE_TRACK_RESOURCE
	if (m_pResource)
		LOG_MESSAGE(RESOURCE_LOAD, m_id.c_str());
#endif

	return m_pResource ? true : false;
}


/******************************************************************************
 * AudioResource::_AutoRelease -- Release the resource.                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
void AudioResource::_AutoRelease()
{
	_DELETE(m_pResource);
	
#ifdef DGE_TRACK_RESOURCE
	LOG_MESSAGE(RESOURCE_UNLOAD, m_id.c_str());
#endif
}


/******************************************************************************
 * AudioResource::_LoadSound -- Load the sound.                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return the loaded sound.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
AbstractSound* AudioResource::_LoadSound(XMLElement* node)
{
/*
**	<Resource type="" id="">
**		<Mono src=""></Mono>	<- node
**	</Resource>
**	<Resource type="audio" id="">
**		<PlayList shuffle="">							<- node
**			<li src=""></li>
**			<li src=""></li>
**			<li src=""></li>
**		</PlayList>
**	</Resource>
*/
	const char* name = node->Name();
	AbstractSound* sound = nullptr;
	if (_STR_SAME(name, "Mono"))
		sound = new MonoSound();
	else if (_STR_SAME(name, "PlayList"))
		sound = new PlayList();
	
	if (sound)
	{
		if (sound->Load(node))
			return sound;
		else
			delete sound;
	}

	return nullptr;
}


/******************************************************************************
 * ImageResource::~ImageResource -- Destructor of the object.                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/13 Tony : Created.                                               *
 *============================================================================*/
ImageResource::~ImageResource()
{
	if (m_pResource)
		_AutoRelease();
}


/******************************************************************************
 * ImageResource::GetResource -- Get image resource.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return image resource.                                            *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/13 Tony : Created.                                               *
 *============================================================================*/
IMAGE* ImageResource::GetResource()
{
	if (!m_pResource)
	{
		Load();
		if (!m_pResource)	// Load failed.
		{
			LOG_ERROR(FAILED_TO_LOAD_RESOURCE, m_id.c_str());
			return nullptr;
		}
	}

	return m_pResource;
}


/******************************************************************************
 * ImageResource::AssignInternal -- Assign internal resource.                 *
 *                                                                            *
 *    Here, a pointer to image is needed, and it must come from operator new. *
 *                                                                            *
 * INPUT:   pResource                                                         *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/13 Tony : Created.                                               *
 *============================================================================*/
void ImageResource::AssignInternal(void* pResource)
{
	m_pResource = static_cast<IMAGE*>(pResource);
}


/******************************************************************************
 * ImageResource::Load -- Load image resource.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/13 Tony : Created.                                               *
 *============================================================================*/
bool ImageResource::Load()
{
	if (m_pResource)
		return true;

	if (!m_xmlNode)
	{
		LOG_ERROR(MISSING_REFERENCE, m_id.c_str());
		return false;
	}

	m_pResource = _LoadImage(m_xmlNode);

#ifdef DGE_TRACK_RESOURCE
	if (m_pResource)
		LOG_MESSAGE(RESOURCE_LOAD, m_id.c_str());
#endif

	return m_pResource ? true : false;
}


/******************************************************************************
 * ImageResource::_AutoRelease -- Release resource.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/13 Tony : Created.                                               *
 *============================================================================*/
void ImageResource::_AutoRelease()
{
	_DELETE(m_pResource);

#ifdef DGE_TRACK_RESOURCE
	LOG_MESSAGE(RESOURCE_UNLOAD, m_id.c_str());
#endif
}


/******************************************************************************
 * ImageResource::_LoadImage -- Load image resource.                          *
 *                                                                            *
 *    Image can be resized. If width and height is 0, then it kept the source *
 *    size.                                                                   *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return the loaded image.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
IMAGE* ImageResource::_LoadImage(XMLElement* node)
{
/*
**	<Resource type="" id="">
**		<img src="" origin="" width="" height="">		<- node
*		</img>
**	</Resource>
*/
	const char* name = node->Name();
	const char* src;

	src = node->Attribute("src");
	if (!src)
	{
		LOG_ERROR(MISSING_ATTRIBUTE, "src", name);
		return nullptr;
	}

	IMAGE* pImage = new IMAGE();
	const char* attr;
	int width = 0;
	int height = 0;
	Coordinate origin;
	Coordinate size;

	_PARSE("width", width, name, 0);
	_PARSE("height", height, name, 0);
	_PARSE_PRIVATE("origin", origin, name, ParseCoord);
	_PARSE_PRIVATE("size", size, name, ParseCoord);
	
	if (size == COORD_ZERO)
	{
		if (FetchImage(pImage, src, width, height))
			return pImage;
		else
		{
			delete pImage;
			return nullptr;
		}
	}
	else
	{
		if (FetchImage(pImage, src, width, height, origin.x, origin.y, size.x, size.y))
			return pImage;
		else
		{
			delete pImage;
			return nullptr;
		}
	}
}


/******************************************************************************
 * MotionResource::~MotionResource -- Destructor of the object.               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
MotionResource::~MotionResource()
{
	if (m_pResource)
		_AutoRelease();
}


/******************************************************************************
 * MotionResource::GetResource -- Get motion resource.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return motion resource.                                           *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
MotionSet* MotionResource::GetResource()
{
	if (!m_pResource)
	{
		Load();
		if (!m_pResource)	// Load failed.
		{
			LOG_ERROR(FAILED_TO_LOAD_RESOURCE, m_id.c_str());
			return nullptr;
		}
	}

	return m_pResource;
}


/******************************************************************************
 * MotionResource::AssignInternal -- Assign internal resoruce.                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   pResource                                                         *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void MotionResource::AssignInternal(void* pResource)
{
	m_pResource = static_cast<MotionSet*>(pResource);
}


/******************************************************************************
 * MotionResource::Load -- Load motion resource.                              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
bool MotionResource::Load()
{
	if (m_pResource)
		return true;

	if (!m_xmlNode)
	{
		LOG_ERROR(MISSING_REFERENCE, m_id.c_str());
		return false;
	}

	m_pResource = _LoadMotion(m_xmlNode);

#ifdef DGE_TRACK_RESOURCE
	if (m_pResource)
		LOG_MESSAGE(RESOURCE_LOAD, m_id.c_str());
#endif

	return m_pResource ? true : false;
}


/******************************************************************************
 * MotionResource::_AutoRelease -- Release the resource.                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void MotionResource::_AutoRelease()
{
	_DELETE(m_pResource);

#ifdef DGE_TRACK_RESOURCE
	LOG_MESSAGE(RESOURCE_UNLOAD, m_id.c_str());
#endif
}


/******************************************************************************
 * MotionResource::_LoadMotion -- Load motion resource.                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return the loaded motionset.                                      *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
MotionSet* MotionResource::_LoadMotion(XMLElement* node)
{
/*
**	<Resource type="motion" id="">
**		<Animatino>				<- node
**			<motion>...</motion>
**			<motion>...</motion>
**		</Animation>
**	</Resource>
*/
	MotionSet* res = new MotionSet();

	if (!res->Load(node))
	{
		delete res;
		return nullptr;
	}

	return res;
}


/******************************************************************************
 * LoadResourceItem -- Load a resource reference.                             *
 *                                                                            *
 *    This only keep the resource item, yet actually load it.                 *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return the new resource.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
AbstractResource* LoadResourceItem(XMLElement* node)
{
	/*
	**	<Resource type="" id="">
	*		<Image></image>			//
	*		<Audio></audio>			// These names are not fixed.
	*		<Motion></motion>		//
	**	</Resource>
	*/
	if (!node)
		return nullptr;

	const char* name = node->Name();
	AbstractResource* res = nullptr;
	XMLElement* ref;
	const char* type;
	const char* id;

	_CHECK_TAG("Resource");

	type = node->Attribute("type");
	if (!type)
	{
		LOG_ERROR(MISSING_ATTRIBUTE, "type", name);
		return nullptr;
	}
	id = node->Attribute("id");
	if (!id)
	{
		LOG_ERROR(MISSING_ATTRIBUTE, "id", name);
		return nullptr;
	}

	ref = node->FirstChildElement();
	if (!ref)
	{
		LOG_ERROR(MISSING_REFERENCE, id);
		return nullptr;
	}

	if (_STR_SAME(type, "image"))
		res = new ImageResource(id);
	else if (_STR_SAME(type, "motion"))
		res = new MotionResource(id);
	else if (_STR_SAME(type, "audio"))
		res = new AudioResource(id);
	else
		LOG_ERROR(INVALID_ATTRIBUTE, "type", name);

	if (res)
		res->AssignExternal(ref);

	return res;
}


/******************************************************************************
 * AssignResourceItem -- Assign internal resource.                            *
 *                                                                            *
 *    Internal resource doesn't have xml reference, so they are always loaded *
 *                                                                            *
 * INPUT:   type      -- Resource type.                                       *
 *          id        -- Resource identifier.                                 *
 *          pResource -- The resource.                                        *
 *                                                                            *
 * OUTPUT:  Return the internal resource.                                     *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
AbstractResource* MakeResourceItem(ResourceType type, const std::string& id, void* pResource)
{
	AbstractResource* res = nullptr;

	switch (type)
	{
	case ResourceType::RES_IMAGE:
		res = new ImageResource(id);
		break;
	case ResourceType::RES_MOTION:
		res = new MotionResource(id);
		break;
	case ResourceType::RES_AUDIO:
		res = new AudioResource(id);
		break;
	default:
		break;
	}

	if (res)
		res->AssignInternal(pResource);

	return res;
}



/******************************************************************************
 * Explorer::Load -- Load resource configuration.                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   filename -- xml file name.                                        *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
bool Explorer::Load(const char* filename)
{
	UnLoad();

	if (!m_config.Load(filename))
		return false;

	_Load();

	m_isLoaded = true;

	_RETURN_STATE();
}


/******************************************************************************
 * Explorer::UnLoad -- Unload all resources.                                  *
 *                                                                            *
 *    This will unload all resources, and close the xml file.                 *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Explorer::UnLoad()
{
	if (!m_isLoaded)
		return;

	m_config.UnLoad();

	_UnLoad();

	m_isLoaded = false;
}


/******************************************************************************
 * Explorer::GetResource -- Get resource item.                                *
 *                                                                            *
 *    This only get the abstract resource, the caller have to get specific    *
 *    resource manually.                                                      *
 *                                                                            *
 * INPUT:   id                                                                *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
AbstractResource* Explorer::GetResource(const std::string& id)
{
	auto it = m_pool.find(id);

	if (it != m_pool.end())
		return it->second;

	return nullptr;
}


/******************************************************************************
 * Explorer::LoadResource -- Load existing item.                              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   id                                                                *
 *                                                                            *
 * OUTPUT:  Return state.                                                     *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
bool Explorer::LoadResource(const std::string& id)
{
	auto it = m_pool.find(id);

	if (it == m_pool.end())
		return false;

	return it->second->Load();
}


/******************************************************************************
 * Explorer::ReleaseResource -- Release existing item.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   id                                                                *
 *                                                                            *
 * OUTPUT:  Return state.                                                     *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
bool Explorer::ReleaseResource(const std::string& id)
{
	auto it = m_pool.find(id);

	if (it == m_pool.end())
		return false;

	it->second->Release();

	return true;
}


/******************************************************************************
 * Explorer::AssignInternalResource -- Add internal resource.                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   type                                                              *
 *          id                                                                *
 *          pResource                                                         *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
bool Explorer::AssignInternalResource(ResourceType type, const std::string& id, void* pResource)
{
	if (m_pool.find(id) != m_pool.end())
	{
		LOG_ERROR(RESOURCE_CONFLICT, id.c_str());
		return false;
	}

	AbstractResource* res = MakeResourceItem(type, id, pResource);
	if (!res)
	{
		LOG_ERROR(CANNOT_MAKE_RESOURCE, id.c_str());
		return false;
	}

#ifdef DGE_TRACK_RESOURCE
	LOG_MESSAGE(RESOURCE_LOAD, id.c_str());
#endif

	m_pool.insert(std::make_pair(id, res));

	return true;
}


/******************************************************************************
 * Explorer::~Explorer -- Destructor of the object.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
Explorer::~Explorer()
{
	UnLoad();
}


/******************************************************************************
 * Explorer::_AddResource -- Add resource to the pool.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   resource                                                          *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
bool Explorer::_AddResource(AbstractResource* resource)
{
	if (!resource)
		return false;

	auto it = m_pool.find(resource->GetIdentifier());
	if (it == m_pool.end())
	{
		m_pool.emplace(resource->GetIdentifier(), resource);
		return true;
	}
	LOG_ERROR(RESOURCE_CONFLICT, resource->GetIdentifier().c_str());

	return false;
}


/******************************************************************************
 * Explorer::_UnLoad -- Actually load the explorer.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Explorer::_Load()
{
/*
** The xml layout.
**	<Config>
**		<Resource type="" id=""></Resource>
**		<Resource type="" id=""></Resource>
**		...
**	</Config>
*/
	XMLElement* node;

	node = m_config.GetRoot()->FirstChildElement();
	while (node)
	{
		_AddResource(LoadResourceItem(node));

		node = node->NextSiblingElement();
	}
}


/******************************************************************************
 * Explorer::_UnLoad -- Actually unload the explorer.                         *
 *                                                                            *
 *    This not only releases all resources, it alse destroys all of them.     *
 *    However, the xml file should be unloaded elsewhere.                     *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Explorer::_UnLoad()
{
	for (auto it = m_pool.begin(); it != m_pool.end(); it++)
		delete it->second;

	m_pool.clear();
}