//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_texture_env_combine_extension.cpp
//	EXT_texture_env_combine extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "GL/glut.h"
#include "../Extensions/glext.h"
#include "../Extensions/wglext.h"
#include "EXT_texture_env_combine_extension.h"

bool EXT_texture_env_combine_supported=false;

bool SetUpEXT_texture_env_combine()
{
	//Check for support
	EXT_texture_env_combine_supported=glutExtensionSupported("GL_EXT_texture_env_combine")!=0;
	
	if(!EXT_texture_env_combine_supported)
	{
		printf("EXT_texture_env_combine unsupported!\n");
		return false;
	}

	printf("EXT_texture_env_combine supported!\n");

	//get function pointers
	//none specified

	return true;
}
