/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Flashback.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : December 8, 2022                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For game flashback. Hmm... Save current progress. :)                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _FLASHBACK_H_
#define _FLASHBACK_H_

#include <dungine.h>


class Flashback : public Pipe
{
public:
	Flashback();
	virtual ~Flashback() {}

	virtual bool Load();
	virtual bool Save();

public:
	void SetHeroName(const std::string& name) { m_heroName = name; }
	const std::string& GetHeroName() const { return m_heroName; }

	void SetHP(int hp)			{ m_hp = hp; }
	int	 GetHP() const			{ return m_hp; }
	void SetMP(int mp)			{ m_mp = mp; }
	int	 GetMP() const			{ return m_mp; }
	void SetArmor(int armor)	{ m_armor = armor; }
	int	 GetArmor() const		{ return m_armor; }
	void SetChi(int chi)		{ m_chi = chi; }
	int	 GetChi() const			{ return m_chi; }
	void SetWeaponList(std::vector<std::string> weaponList) { m_weaponList = weaponList; }
	std::vector<std::string>& GetWeaponList() { return m_weaponList; }

	void SetChapter	(int chapter)		{ m_chapter = chapter; }
	int	 GetChapter() const				{ return m_chapter; }
	void SetLevel(int level)			{ m_level = level; }
	int	 GetLevel() const				{ return m_level; }
	void SetInfinite(bool isInfinite)	{ m_isInfinite = isInfinite; }
	bool GetInfinite() const			{ return m_isInfinite; }

	void IsValid(bool isValid)	{ m_isValid = isValid; }
	bool IsValid() const		{ return m_isValid; }

private:
	void _LoadWeaponList(XMLFile& file);
	void _SaveWeaponList(XMLFile& file);

private:
	// Hero status.
	std::string m_heroName;
	int m_hp;
	int m_mp;
	int m_armor;
	int m_chi;
	std::vector<std::string> m_weaponList;

	// Chapter info.
	int m_chapter;
	int m_level;
	bool m_isInfinite;

	// Valid flag.
	bool m_isValid;
};


#endif