#include "node-api-helpers.h"
#include <GLFW/glfw3.h>


#include "vr.h"

Hmd hmd;

// attrLoc, components, type, normalize, stride, offset
napi_value vrConnect(napi_env env, napi_callback_info info) {
	napi_status status = napi_ok;
	napi_value args[1];
	size_t argc = checkArgCount(env, info, args, 1, 0);

	bool enable = getBool(env, args[0]);

	bool ok = true;
	if (enable) {
		ok = hmd.connect();
	} else {
		hmd.disconnect();
	}
	napi_value result_value;
	status = napi_create_uint32(env, ok ? 1 : 0, &result_value);
	return (status == napi_ok) ? result_value : nullptr;
}


napi_value vrGetTextureWidth(napi_env env, napi_callback_info info) {
	napi_status status = napi_ok;
	napi_value args[1];
	size_t argc = checkArgCount(env, info, args, 1, 0);

	uint32_t v = 0; 
	if (hmd.connected) {
		v = hmd.width;
	}
	napi_value result_value;
	status = napi_create_uint32(env, v, &result_value);
	return (status == napi_ok) ? result_value : nullptr;
}

napi_value vrGetProjection(napi_env env, napi_callback_info info) {
	napi_status status = napi_ok;
	napi_value args[2];
	size_t argc = checkArgCount(env, info, args, 2, 2);
	int eye = getInt32(env, args[0]);

	bool isArray;
	status = napi_is_array(env, args[1], &isArray);

	if (isArray) {
		glm::mat4& mat = hmd.mProjMatEye[eye];
		napi_value value;

		for (int i=0; i<16; i++) {
			status = napi_create_double(env, mat[i / 4][i % 4], &value);
			status = napi_set_element(env, args[1], i, value);
		}
	} else {
		napi_throw_type_error(env, nullptr, "needs an array to fill");
	}
	
	return args[1];
}


napi_value vrGetTextureHeight(napi_env env, napi_callback_info info) {
	napi_status status = napi_ok;
	napi_value args[1];
	size_t argc = checkArgCount(env, info, args, 1, 0);

	uint32_t v = 0; 
	if (hmd.connected) {
		v = hmd.height;
	}
	napi_value result_value;
	status = napi_create_uint32(env, v, &result_value);
	return (status == napi_ok) ? result_value : nullptr;
}

napi_value vrUpdate(napi_env env, napi_callback_info info) {
	napi_status status = napi_ok;
	napi_value args[1];
	size_t argc = checkArgCount(env, info, args, 1, 0);

	if (hmd.connected) {
		hmd.update();
	}

	return nullptr;
}

napi_value vrSubmit(napi_env env, napi_callback_info info) {
	napi_status status = napi_ok;
	napi_value args[1];
	size_t argc = checkArgCount(env, info, args, 1, 0);

	uint32_t texid = getUint32(env, args[0]);
	if (hmd.connected) {
		hmd.submit(texid);
	}

	bool ok = true;
	// TODO
	napi_value result_value;
	status = napi_create_uint32(env, ok ? 1 : 0, &result_value);
	return (status == napi_ok) ? result_value : nullptr;
}