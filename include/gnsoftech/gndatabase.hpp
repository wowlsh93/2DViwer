#pragma once

#include	"gnsoftech/gndatalink.hpp"

class GNDBase
{
protected:
	MSXML2::IXMLDOMDocumentPtr		m_xmlDoc;

public:
	inline GNDBase()
	{
	}

	inline ~GNDBase()
	{
	}
	
	inline bool load(const std::wstring& strXMLConfig)
	{
		return true;
	}
	
	inline bool loadXML(const std::wstring& strXML)
	{
		return true;
	}


};
