#include "Function.h"
#include "pch.h"
using namespace std;


int HextoDec(string hex)
{
	int len = hex.size();
	int dec = 0;

	for (int i = 0; i < len; i++) {

		if (hex[i] >= '0' && hex[i] <= '9')
			dec += (int(hex[i]) - 48) * pow(16, len - 1 - i);

		else if (hex[i] >= 'A' && hex[i] <= 'F')
			dec += (int(hex[i]) - 55) * pow(16, len - 1 - i);
	}
	return dec;
}

char*getDomain(char*link) {

	char*OriginalSite = new char[strlen(link) + 1];
	strcpy(OriginalSite, link);

	char*TempDomain;
	TempDomain = strtok(OriginalSite, "/");
	TempDomain = strtok(NULL, "/");
	char*Domain = new char[strlen(TempDomain) + 1];
	strcpy(Domain, TempDomain);

	delete[] OriginalSite;

	return Domain;
}

char*getRequest(char*link) {

	char*OriginalSite = new char[strlen(link) + 1];
	strcpy(OriginalSite, link);

	char*TempRequest;
	TempRequest = strtok(OriginalSite, "/");
	TempRequest = strtok(NULL, "/");
	TempRequest = strtok(NULL, "");

	char*Request = NULL;
	if (TempRequest != NULL)
	{
		Request = new char[strlen(TempRequest) + 1];
		strcpy(Request, TempRequest);
	}
	else
	{
		Request = new char[2];
		memset(Request, 0, 2);
	}

	delete[] OriginalSite;

	return Request;
}

char*getFilename(char*Request) {

	char*Filename = NULL;
	if (strcmp(Request, "") != 0)
	{
		char*OriginalRequest = new char[strlen(Request) + 1];
		strcpy(OriginalRequest, Request);

		char*Tempfilename;
		Tempfilename = strtok(OriginalRequest, "/");
		char*Tempfilename2 = NULL;

		while (Tempfilename != NULL)
		{
			Tempfilename2 = Tempfilename;
			Tempfilename = strtok(NULL, "/");
		}

		Filename = new char[strlen(Tempfilename2) + 1];
		strcpy(Filename, Tempfilename2);

		delete[] OriginalRequest;

	}
	else
	{
		Filename = new char[2];
		memset(Filename, 0, 2);
	}

	return Filename;
}

wchar_t* getIP(char*Domain) {

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	struct hostent *he = gethostbyname(Domain);
	if (he == NULL)
		return NULL;

	char *ip = inet_ntoa(*(struct in_addr*)he->h_addr_list[0]);
	if (ip == NULL)
		return NULL;

	// Chuyen ip (char*) thanh wip (LPCTSTR) de phu hop voi tham so truyen vao ham Connect
	wchar_t *wip = new wchar_t[strlen(ip) + 1];
	mbstowcs(wip, ip, strlen(ip) + 1);
	return wip;
}

char* getClientMsg(char*Request, char*Domain) {
	string str1 = "GET /";
	string str2(Request);
	string str3 = " HTTP/1.1\r\nHost: ";
	string str4(Domain);
	string str5 = "\r\nConnection: keep-alive\r\n\r\n";
	string sClientMsg = str1 + str2 + str3 + str4 + str5;

	char* ClientMsg = strcpy(new char[sClientMsg.length() + 1], sClientMsg.c_str());
	ClientMsg[sClientMsg.length()] = '\0';

	return ClientMsg;
}

int getHtmlType(char Msg[], CSocket& ClientSocket) {
	for (;;)
	{
		ClientSocket.Receive(Msg, 1, 0);

		if (Msg[0] == 'C') {

			ClientSocket.Receive(Msg, 1, 0);
			if (Msg[0] == 'o')
			{
				ClientSocket.Receive(Msg, 1, 0);
				if (Msg[0] == 'n')
				{
					ClientSocket.Receive(Msg, 1, 0);
					if (Msg[0] == 't')
					{
						ClientSocket.Receive(Msg, 1, 0);
						if (Msg[0] == 'e')
						{
							ClientSocket.Receive(Msg, 1, 0);
							if (Msg[0] == 'n')
							{
								ClientSocket.Receive(Msg, 1, 0);
								if (Msg[0] == 't')
								{
									ClientSocket.Receive(Msg, 1, 0);
									if (Msg[0] == '-')
									{
										ClientSocket.Receive(Msg, 1, 0);
										if (Msg[0] == 'L')
										{
											memset(Msg, 0, MAXLENGTH + 1);
											ClientSocket.Receive(Msg, 7, 0);

											return 1;
											break;
										}
									}

								}
							}
						}
					}
				}
			}
		}
		else if (Msg[0] == 'c')
		{
			ClientSocket.Receive(Msg, 1, 0);
			if (Msg[0] == 'h')
			{
				ClientSocket.Receive(Msg, 1, 0);
				if (Msg[0] == 'u')
				{
					ClientSocket.Receive(Msg, 1, 0);
					if (Msg[0] == 'n')
					{
						ClientSocket.Receive(Msg, 1, 0);
						if (Msg[0] == 'k')
						{
							ClientSocket.Receive(Msg, 1, 0);
							if (Msg[0] == 'e')
							{
								ClientSocket.Receive(Msg, 1, 0);
								if (Msg[0] == 'd')
								{
									return 2;
									break;
								}
							}
						}
					}
				}
			}
		}
		memset(Msg, 0, MAXLENGTH + 1);
	}
}

int openFile(ofstream& f, char*Request, char*Domain, char*Filename, int&folder, int HtmlType, string& foldername, char*cmdl) {

	if (strcmp(Request, "") == 0) {

		string name1(Domain);
		string name2 = "_index.html";
		string name = name1 + name2;
		f.open(name, ios::binary);
		if (!f)
		{
			cout << "\nKhong the mo file " << name << endl;
			return 0;
		}
		folder = 0;
		return 1;
	}

	else {

		string name1(Domain);
		string name2(Filename);
		string name = name1 + "_" + name2;

		//Neu khong tim thay dau "." trong request (nghia la se xet truong hop luu folder)
		if (name2.find(".") == -1)
		{
			//Neu co dau "/"
			if (cmdl[strlen(cmdl) - 1] == '/')
			{
				if (mkdir(name.c_str()) == -1)
					cout << "\nKhong the tao thu muc " << name << endl;

				else
					cout << "\nThu muc \"" << name << "\" da duoc tao!" << endl;

				folder = 1;
				foldername = name;

				//Tao them file .html de luu noi dung cua file .html chua cac subfile
				name = name + ".html";
				f.open(name, ios::binary);
				if (!f)
				{
					cout << "\nKhong the mo file " << name << endl;
					return 0;
				}
				return 1;
			}

			//Neu ko co dau "/"
			else
			{
				name = name + ".html";
				f.open(name, ios::binary);
				if (!f)
				{
					cout << "\nKhong the mo file " << name << endl;
					return 0;
				}
				folder = 0;
				return 1;
			}

		}
		else {

			f.open(name, ios::binary);
			if (!f)
			{
				cout << "\nKhong the mo file " << name << endl;
				return 0;
			}
			folder = 0;
			return 1;
		}

	}
	return 0;
}

void receiveContentLengthHTML(ofstream& f, char Msg[], CSocket& ClientSocket) {

	memset(Msg, 0, MAXLENGTH + 1);
	int ByteRead = ClientSocket.Receive(Msg, MAXLENGTH, 0);
	Msg[ByteRead] = '\0';

	char *k = Msg;
	int ContentLength = atoi(k);

	k = strstr(Msg, "\r\n\r\n");
	if (k != NULL)
	{
		k = k + 4;
		ByteRead = ByteRead - (k - Msg);
		f.write(k, ByteRead);

		ContentLength = ContentLength - ByteRead;

	}
	else
	{
		f.write(Msg, ByteRead);
		ContentLength = ContentLength - ByteRead;

	}

	while (ContentLength > 0)
	{
		memset(Msg, 0, MAXLENGTH + 1);
		ByteRead = ClientSocket.Receive(Msg, MAXLENGTH, 0);
		Msg[ByteRead] = '\0';

		if (ContentLength >= ByteRead) {
			f.write(Msg, ByteRead);
		}

		else {
			Msg[ContentLength] = '\0';
			f.write(Msg, ContentLength);
		}

		ContentLength = ContentLength - ByteRead;

	}
	f.close();//

}

void receiveChunkedHTML(ofstream& f, char Msg[], CSocket& ClientSocket) {

	memset(Msg, 0, 2);

	for (; ; )
	{
		ClientSocket.Receive(Msg, 1, 0);
		if (Msg[0] == '\r')
		{
			ClientSocket.Receive(Msg, 1, 0);
			if (Msg[0] == '\n')
			{
				ClientSocket.Receive(Msg, 1, 0);
				if (Msg[0] == '\r')
				{
					ClientSocket.Receive(Msg, 1, 0);
					if (Msg[0] == '\n')
					{
						break;
					}
				}
			}
		}
	}

	string hex = "";
	int numByte = 0;
	char* Message = NULL;

	ClientSocket.Receive(Msg, 1, 0);
	char c = Msg[0];
	ClientSocket.Receive(Msg, 1, 0);
	char b = Msg[0];

	while (c != '0' && b != '\r')
	{
		hex.push_back(toupper(c));
		hex.push_back(toupper(b));

		ClientSocket.Receive(Msg, 1, 0);
		c = Msg[0];

		while (c != '\r') {
			hex.push_back(toupper(c));
			ClientSocket.Receive(Msg, 1, 0);
			c = Msg[0];
		}

		ClientSocket.Receive(Msg, 1, 0);

		numByte = HextoDec(hex);

		Message = new char[numByte + 1];
		memset(Message, 0, numByte + 1);

		int byteRead = ClientSocket.Receive(Message, numByte, 0);
		f.write(Message, byteRead);

		while (byteRead < numByte)
		{
			memset(Message, 0, numByte + 1);
			numByte -= byteRead;
			byteRead = ClientSocket.Receive(Message, numByte, 0);
			f.write(Message, byteRead);

		}

		ClientSocket.Receive(Msg, 1, 0); //skip \r\n
		ClientSocket.Receive(Msg, 1, 0);

		ClientSocket.Receive(Msg, 1, 0); //lay tiep de chay while
		c = Msg[0];

		ClientSocket.Receive(Msg, 1, 0);
		b = Msg[0];

		hex = "";
		delete[] Message;


	}
	f.close();//
}

void DownloadFolder(char* cFolderLink, CSocket& ClientSocket)
{
	// get folder html name
	// folder: domain_filename
	// file html content - length: domain_filename.html
	char* Domain = getDomain(cFolderLink);
	char* Filename = getFilename(cFolderLink);
	string name1(Domain);
	string name2(Filename);
	string dirName = name1 + "_" + name2;
	string sFolderLink(cFolderLink);
	string sFileLink;
	ifstream f;
	f.open(dirName + ".html", ios::binary);
	if (!f)
	{
		cout << "\nKhong mo duoc folder.html\n";
		return;
	}



	// read folder html
	// file line: each line of file
	string fileLine;
	while (getline(f, fileLine))
	{

		// collect link and download files
		while (fileLine.find("href") != string::npos)
		{
			// get inner filename
			// VD: href="01-intro.pdf"
			fileLine.erase(0, fileLine.find("href") + 6); // delete trash: href=" -> 6
			string fname = fileLine.substr(0, fileLine.find("\""));
			// check file name
			if (fname.find(".") == string::npos)
				continue;



			// get link to inner file
			// VD: http://web.stanford.edu/class/cs224w/slides/01-intro.pdf
			if (sFolderLink[sFolderLink.size() - 1] == '/')
				sFileLink = sFolderLink + fname;
			else
				sFileLink = sFolderLink + "/" + fname;
			// convert to char*
			char* cFileLink = new char[sFileLink.size()];
			strcpy(cFileLink, sFileLink.c_str());



			//Gui request den server
			//"GET / HTTP/1.1\r\nHost: example.com\r\nConnection: keep-alive\r\n\r\n"
			char* ClientMsg = getClientMsg(getRequest(cFileLink), Domain);
			ClientSocket.Send(ClientMsg, strlen(ClientMsg), 0);



			// get htmltype
			char Msg[MAXLENGTH + 1];
			memset(Msg, 0, MAXLENGTH + 1);
			int HtmlType = getHtmlType(Msg, ClientSocket);




			// handle filename correction
			string temp_fname = fname;
			int index = temp_fname.find("%20");
			if (index != string::npos) //Tim thay %20 trong ten file thi thay thành dấu cách
				temp_fname.replace(index, 3, " ");




			// open file VD: example_slices\\data.pdf
			ofstream fi;
			fi.open(dirName + "\\" + temp_fname, ios::binary);
			if (!fi)
			{
				cout << "Khong the mo file " << fname;
				continue;
			}


			// Neu la content-length
			if (HtmlType == 1)
				receiveContentLengthHTML(fi, Msg, ClientSocket);
			//Neu la dang chunked
			else if (HtmlType == 2)
				receiveChunkedHTML(fi, Msg, ClientSocket);

			else //2 hàm trên close file rồi
				fi.close();

			delete[] ClientMsg;

		}
	}



	f.close();
	delete[] Domain;
	delete[] Filename;



	// delete dirName.html
	string baseHtmlFile = dirName + ".html";
	remove(baseHtmlFile.c_str());
}

void Download(char* cmdl)
{
	// Lay domain tu argv
	char* Domain = getDomain(cmdl);

	// Lay request tu argv
	char* Request = getRequest(cmdl);

	// Lay du lieu ten file can download tu request
	char* Filename = getFilename(Request);

	// Chuyen domain thanh IP
	wchar_t* wip = getIP(Domain);
	if (wip == NULL) {
		delete[] Domain;
		delete[] Request;
		delete[] Filename;
		cout << "\nKhong tim thay IP\n";
		return;
	}

	// Khoi tao socket 
	CSocket ClientSocket;
	if (ClientSocket.Create(0, SOCK_STREAM, NULL) == 0) { // Truyen vao tham so port la 0 de MFC tao port ngau nhien
		cout << "\nKhoi tao socket that bai\n";
		cout << ClientSocket.GetLastError() << endl;
		return;
	}

	// Ket noi den server
	if (ClientSocket.Connect(wip, 80) != 0)
	{
		cout << "\nKet noi thanh cong den Server !\n";


		//Gui request den server

							//"GET / HTTP/1.1\r\nHost: example.com\r\nConnection: keep-alive\r\n\r\n"	
		char* ClientMsg = getClientMsg(Request, Domain);
		ClientSocket.Send(ClientMsg, strlen(ClientMsg), 0);


		// Bat dau nhan msg

		char Msg[MAXLENGTH + 1];
		memset(Msg, 0, MAXLENGTH + 1);


		//Tim loai html (1: content-length, 2: chunked)

		int HtmlType = getHtmlType(Msg, ClientSocket);

		// Mo file luu

		int folder = -1; //Tao bien folder de biet la luu folder hay luu file (0: luu file, 1: luu folder)
		ofstream f;
		string foldername = ""; // Tao string foldername de luu ten folder neu mo folder

		if (openFile(f, Request, Domain, Filename, folder, HtmlType, foldername, cmdl) == 0)
			return;

		/* Lưu ý: lần lượt gọi cả 3 hàm getHtmlType -> openFile -> receiveContentLengthHTML /  receiveChunkedHTML
		mỗi lần muốn nhận data của file nào*/

		// Neu la content-length
		if (HtmlType == 1) {
			receiveContentLengthHTML(f, Msg, ClientSocket);
		}

		//Neu la dang chunked
		else if (HtmlType == 2) {
			receiveChunkedHTML(f, Msg, ClientSocket);
		}

		//Neu co folder, thuc hien goi ham tai tat ca cac subfile trong folder ve
		if (folder == 1) {
			DownloadFolder(cmdl, ClientSocket);
		}

		delete[] ClientMsg;

	}
	else
	{
		cout << "\nKhong the ket noi den web server\n";
	}

	// Dong ket noi
	ClientSocket.Close();

	delete[] wip;
	delete[] Domain;
	delete[] Request;
	delete[] Filename;

	cout << "\nCompleted\n";
}

