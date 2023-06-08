#pragma once
using namespace std;
int HextoDec(string hex);
char*getDomain(char*link);
char*getRequest(char*link);
char*getFilename(char*Request);
wchar_t* getIP(char*Domain);
char* getClientMsg(char*Request, char*Domain);
int getHtmlType(char Msg[], CSocket& ClientSocket);
int openFile(ofstream& f, char*Request, char*Domain, char*Filename, int&folder, int HtmlType, string& foldername, char*cmdl);
void receiveContentLengthHTML(ofstream& f, char Msg[], CSocket& ClientSocket);
void receiveChunkedHTML(ofstream& f, char Msg[], CSocket& ClientSocket);
void DownloadFolder(char* cFolderLink, CSocket& ClientSocket);
void Download(char* cmdl);