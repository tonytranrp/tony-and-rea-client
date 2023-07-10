﻿#include "ChatSuffix.h"

#include "../../../../Utils/Utils.h"
ChatSuffix::ChatSuffix() : IModule(0, Category::OTHER, "Adds the client suffix") {
}

ChatSuffix::~ChatSuffix() {
}

const char* ChatSuffix::getModuleName() {
	return "ChatSuffix";
}

void ChatSuffix::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_TextPacket>()) {
		C_TextPacket* funy = reinterpret_cast<C_TextPacket*>(packet);
		std::string Sentence;
		std::string start;
		std::string end;
		std::string release;
		start = "> ";
		release = "Paid";
#ifdef _DEBUG
		release = "Dev";
#endif
		end = " | REA! ON TOP!  #GawrGura ON TOP! | Release: -" + release + " -|- " + Utils::randomString(8) + "| --" + "REA IS SEXY!!--|" + Utils::randomString(23);
		Sentence = start + funy->message.getText() + end;
		funy->message.resetWithoutDelete();
		funy->message = Sentence;
	}
}