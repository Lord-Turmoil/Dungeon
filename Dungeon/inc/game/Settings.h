/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Settings.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 10, 2022                             *
 *                                                                            *
 *                    Last Update : November 24, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Game settings is a singleton.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <dungine.h>

struct HeroInfo
{
	int price;
	bool isAvailable;
};

class Settings : public Singleton<Settings>
{
	friend class Singleton<Settings>;
public:
	bool Link(const char* filename);
	bool Load();
	bool Save();
	bool SaveSettings();
	bool SaveConfig();

public:
	// Settings
	void IsFullscreen(bool isFullscreen) { m_isFullscreen = isFullscreen; }
	bool IsFullscreen() const { return m_isFullscreen; }
	
	void Difficulty(int difficulty) { m_difficulty = difficulty; }
	int Difficulty() const { return m_difficulty; }
	
	void SoundVolume(double volume) { m_soundVolume = volume; }
	double SoundVolume() const { return m_soundVolume; }
	void MusicVolume(double volume) { m_musicVolume = volume; }
	double MusicVolume() const { return m_musicVolume; }

	// Config
	int Coin() const { return m_coin; }
	void Coin(int coin) { m_coin = coin; }
	void AddCoin(int coin) { m_coin += coin; }
	void HeroName(const std::string& name) { m_heroName = name; }
	const std::string& HeroName() const { return m_heroName; }
	bool IsAvailable(const std::string& name) const;
	bool CanBeAvailable(const std::string& name) const;
	bool MakeAvailable(const std::string& name);
	COLORREF BeginColor() const { return m_beginColor; }
	COLORREF EndColor() const { return m_endColor; }

private:
	Settings();
	~Settings() {}
	
	bool _IsLinked();

	void _LoadEntry(XMLFile& file, int* val, const char* tag);
	void _LoadEntry(XMLFile& file, double* val, const char* tag);
	void _LoadEntry(XMLFile& file, bool* val, const char* tag);
	void _LoadEntry(XMLFile& file, COLORREF* val, const char* tag);
	void _LoadHeroInfo(XMLFile& file);

	void _SaveEntry(XMLFile& file, int val, const char* tag);
	void _SaveEntry(XMLFile& file, double val, const char* tag);
	void _SaveEntry(XMLFile& file, bool val, const char* tag);
	void _SaveEntry(XMLFile& file, COLORREF val, const char* tag);
	void _SaveHeroInfo(XMLFile& file);

private:
	const char* m_filename;

	// Settings
	int m_difficulty;
	double m_soundVolume;
	double m_musicVolume;
	bool m_isFullscreen;

	// Config
	int m_coin;
	std::string m_heroName;
	COLORREF m_beginColor;	// Top color of the sky.
	COLORREF m_endColor;	// Bottom color of the sky.

	std::map<std::string, HeroInfo> m_heroInfo;
};


/********************************************************************
** I'm not very familiar with try-cath machanism, so... give it a
** try? :)
*/
class EntryError
{
public:
	EntryError(const char* entry) : m_entry(entry) {}

	const char* what() const { return m_entry; }

private:
	const char* m_entry;
};

#endif