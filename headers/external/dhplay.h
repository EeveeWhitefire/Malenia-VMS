#ifndef DHPLAY_H
#define DHPLAY_H
#define CLIENT_NET_API  extern "C"
#define CALL_METHOD
#define CALLBACK
#define BYTE unsigned char
#ifdef __cplusplus
extern "C" {
CLIENT_NET_API BOOL CALL_METHOD PLAY_GetFreePort(int* port);
CLIENT_NET_API BOOL CALL_METHOD PLAY_OpenStream(int port, void* a1, int a2, int a3);
CLIENT_NET_API BOOL CALL_METHOD PLAY_Play(int port, void* hwnd);
CLIENT_NET_API BOOL CALL_METHOD PLAY_ReleasePort(int port);
CLIENT_NET_API BOOL CALL_METHOD PLAY_CloseStream(int port);
CLIENT_NET_API BOOL CALL_METHOD PLAY_Stop(int port);
CLIENT_NET_API BOOL CALL_METHOD PLAY_SetDecCallBack(int port, void* callback);
CLIENT_NET_API BOOL CALL_METHOD PLAY_InputData(int port, BYTE*, unsigned int size);
}
#endif
#endif
