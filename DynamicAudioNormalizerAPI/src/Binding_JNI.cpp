//////////////////////////////////////////////////////////////////////////////////
// Dynamic Audio Normalizer - Audio Processing Library
// Copyright (c) 2014 LoRd_MuldeR <mulder2@gmx.de>. Some rights reserved.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// http://www.gnu.org/licenses/lgpl-2.1.txt
//////////////////////////////////////////////////////////////////////////////////

#include "DynamicAudioNormalizer.h"

//StdLib
#include <algorithm>

//JNI
#include <DynamicAudioNormalizer_JDynamicAudioNormalizer.h>
#include <DynamicAudioNormalizer_JDynamicAudioNormalizer_Error.h>
#include <DynamicAudioNormalizer_JDynamicAudioNormalizer_NativeAPI.h>

///////////////////////////////////////////////////////////////////////////////
// Utility Functions
///////////////////////////////////////////////////////////////////////////////

#define JAVA_CHECK_EXCEPTION() do \
{ \
	if(env->ExceptionCheck()) \
	{ \
		env->ExceptionClear(); \
		return JNI_FALSE; \
	} \
} \
while(0) \

#define JAVA_FIND_CLASS(VAR,NAME) do \
{ \
	(VAR) = env->FindClass((NAME)); \
	JAVA_CHECK_EXCEPTION(); \
	if((VAR) == NULL) return JNI_FALSE; \
} \
while(0)

#define JAVA_GET_METHOD(VAR,CLASS,NAME,ARGS) do \
{ \
	(VAR) = env->GetMethodID((CLASS), (NAME), (ARGS)); \
	JAVA_CHECK_EXCEPTION(); \
	if((VAR) == NULL) return JNI_FALSE; \
} \
while(0)

#define JAVA_MAP_PUT(MAP,KEY,VAL) do \
{ \
	jstring _key = env->NewStringUTF((KEY)); \
	jstring _val = env->NewStringUTF((VAL)); \
	JAVA_CHECK_EXCEPTION(); \
	\
	if(_key && _val) \
	{ \
		jobject _ret = env->CallObjectMethod((MAP), putMethod, _key, _val); \
		JAVA_CHECK_EXCEPTION(); \
		if(_ret) env->DeleteLocalRef(_ret); \
	} \
	\
	if(_key) env->DeleteLocalRef(_key); \
	if(_val) env->DeleteLocalRef(_val); \
} \
while(0)

///////////////////////////////////////////////////////////////////////////////
// JNI Functions
///////////////////////////////////////////////////////////////////////////////

extern "C"
{
	JNIEXPORT jboolean JNICALL Java_DynamicAudioNormalizer_JDynamicAudioNormalizer_00024NativeAPI_getVersionInfo(JNIEnv *env, jclass, jintArray versionInfo)
	{
		if(env->GetArrayLength(versionInfo) == 3)
		{
			if(jint *versionInfoElements = env->GetIntArrayElements(versionInfo, JNI_FALSE))
			{
				uint32_t major, minor, patch;
				MDynamicAudioNormalizer::getVersionInfo(major, minor, patch);
			
				versionInfoElements[0] = (int32_t) std::min(major, uint32_t(INT32_MAX));
				versionInfoElements[1] = (int32_t) std::min(minor, uint32_t(INT32_MAX));
				versionInfoElements[2] = (int32_t) std::min(patch, uint32_t(INT32_MAX));

				env->ReleaseIntArrayElements(versionInfo, versionInfoElements, 0);
				return JNI_TRUE;
			}
		}

		return JNI_FALSE;
	}

	JNIEXPORT jboolean JNICALL Java_DynamicAudioNormalizer_JDynamicAudioNormalizer_00024NativeAPI_getBuildInfo(JNIEnv *env, jclass, jobject buildInfo)
	{
		jclass mapClass = NULL;
		jmethodID putMethod = NULL, clearMethod = NULL;

		JAVA_FIND_CLASS(mapClass, "java/util/Map");
		JAVA_GET_METHOD(putMethod, mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		JAVA_GET_METHOD(clearMethod, mapClass, "clear", "()V");

		if(!env->IsInstanceOf(buildInfo, mapClass))
		{
			return JNI_FALSE;
		}

		env->CallVoidMethod(buildInfo, clearMethod);
		JAVA_CHECK_EXCEPTION();

		const char *date, *time, *compiler, *arch;
		bool debug;
		MDynamicAudioNormalizer::getBuildInfo(&date, &time, &compiler, &arch, debug);

		JAVA_MAP_PUT(buildInfo, "BuildDate",    date);
		JAVA_MAP_PUT(buildInfo, "BuildTime",    time);
		JAVA_MAP_PUT(buildInfo, "Compiler",     compiler);
		JAVA_MAP_PUT(buildInfo, "Architecture", arch);
		JAVA_MAP_PUT(buildInfo, "DebugBuild",   debug ? "Yes" : "No");

		env->DeleteLocalRef(mapClass);
		return JNI_TRUE;
	}
}
