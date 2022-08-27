/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungine Engine)                   *
 *                                                                            *
 *                      File Name : Prototype.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 1, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Prototype pattern.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _PROTOTYPE_H_
#define _PROTOTYPE_H_

/********************************************************************
** The object is set to prototype by default. Must manually set to
** none-prototype in Clone().
** Why private? Just for fun. :)
*/
template<typename T>
class Prototype
{
public:
	Prototype() : m_isPrototype(true) {}
	Prototype(const Prototype&) = delete;
	Prototype& operator=(const Prototype&) = delete;
	~Prototype() = default;

	virtual T* Clone() const = 0;
	virtual void Clone(T* clone) const = 0;

protected:
	void _MakePrototype(bool isPrototype) { m_isPrototype = isPrototype; }
	bool _IsPrototype() { return m_isPrototype; }

private:
	bool m_isPrototype;
};

#endif