//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.hpp"

#include "BinaryReader.hpp"

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 


BinaryReader::BinaryReader(void) :
	m_Exists(false),
	m_pReader(nullptr)
{
}

BinaryReader::~BinaryReader(void)
{
	Close();
}

std::string BinaryReader::ReadLongString()
{
	if (m_pReader == nullptr)
		std::cout << "BinaryReader doesn't exist!\nUnable to read binary data..." << std::endl;

	auto stringLength = Read<uint32>();

	std::stringstream ss;
	for (uint32 i = 0; i<stringLength; ++i)
	{
		ss << Read<uint8>();
	}

	return (std::string)ss.str();
}

std::string BinaryReader::ReadNullString()
{
	if (m_pReader == nullptr)
	{
		std::cout << "BinaryReader doesn't exist!\nUnable to read binary data..." << std::endl;
		return "";
	}

	std::string buff;
	getline(*m_pReader, buff, '\0');

	return std::string(buff.begin(), buff.end());
}


std::string BinaryReader::ReadString()
{
	if (m_pReader == nullptr)
		std::cout << "BinaryReader doesn't exist!\nUnable to read binary data..." << std::endl;

	int32 stringLength = (int32)Read<uint8>();

	std::stringstream ss;
	for (int32 i = 0; i<stringLength; ++i)
	{
		ss << Read<uint8>();
	}

	return (std::string)ss.str();
}

void BinaryReader::Open( const std::vector<uint8> &binaryContent )
{
	Close();

	std::string data((char*)binaryContent.data(), binaryContent.size() );
	m_pReader = new std::stringstream( data );
	m_Exists = true;
}

void BinaryReader::Close()
{
	SafeDelete(m_pReader);
	m_Exists = false;
}

int32 BinaryReader::GetBufferPosition()
{
	if (m_pReader)
	{
		return static_cast<int32>(m_pReader->tellg());
	}

	std::cout << "[WARNING] " << L"BinaryReader::GetBufferPosition> m_pReader doesn't exist"<<std::endl;
	return -1;
}

bool BinaryReader::SetBufferPosition(int32 pos)
{
	if (m_pReader)
	{
		m_pReader->seekg(pos);
		return true;
	}

	std::cout << "[WARNING] " << L"BinaryReader::SetBufferPosition> m_pReader doesn't exist" << std::endl;
	return false;
}

bool BinaryReader::MoveBufferPosition(int32 move)
{
	auto currPos = GetBufferPosition();
	if (currPos>0)
	{
		return SetBufferPosition(currPos + move);
	}

	return false;
}
