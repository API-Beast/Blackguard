/*
 *   Copyright (C) 2010-2013 Manuel Riecke <m.riecke@mrbeast.org>
 *
 *   This software is provided 'as-is', without any express or implied
 *   warranty.  In no event will the authors be held liable for any damages
 *   arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *   including commercial applications, and to alter it and redistribute it
 *   freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *   2. Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *   3. This notice may not be removed or altered from any source distribution.
 */
#ifndef CLIARGUMENTINTERPRETER_H
#define CLIARGUMENTINTERPRETER_H

#include <vector>
#include <map>
#include <string>

namespace GRG
{
using namespace std;

class AbstractSwitch;

class SwitchParser
{
public:
	void parseArgv(int argc, char** argv);              /**< Process the arguments. */
	void setAnonymousSwitch(AbstractSwitch& argSwitch); /**< Registers @c argSwitch as anonymous switch. The anonymous switch is populated by arguments which can't be assigned to any other switch.
                                                        *  Example: If <i>coolprogram fooBar.txt</i> is given, the argument \c fooBar.txt will be given to the anonymous switch. */

	void registerSwitch(AbstractSwitch& argSwitch, const std::string& longName, char shortName);
                      /**< Registers< @c argSwitch under the long name @c longName and the short name @c shortName. */
  void registerSwitch(AbstractSwitch& argSwitch, const std::string& longName);
                      /**< Registers< @c argSwitch under the name @c longName. */
private:
	map<std::string, AbstractSwitch*> mSwitches;
	map<char,   AbstractSwitch*> mSwitchesShortName;
	AbstractSwitch* mAnonymousSwitch;
};

/** Base class for all CLI switches.
 *  @see SwitchParser */
class AbstractSwitch
{
public:
	virtual void giveArgument(std::string){}; /**< Will be called if an argument was assigned to this switch.   */
	virtual void set(bool set=true){};        /**< Will be called if this switch was given.                     */
	virtual bool acceptsMoreArguments()=0;    /**< Should return wether this switch accepts any more arguments. */
};

/** An boolean switch. It doesn't take any arguments and can only be either set or not. */
class BooleanSwitch : public AbstractSwitch
{
public:
	BooleanSwitch();
	virtual void set(bool set=true);
	virtual bool acceptsMoreArguments();
public:
	bool isSet; /**< \c true if this switch is active. */
};

/** An switch which only takes one argument. */
class SimpleSwitch : public BooleanSwitch
{
public:
	virtual bool acceptsMoreArguments();
	virtual void giveArgument(std::string argument);
public:
	std::string argument;
};

/** An switch with a variable ammount of arguments. */
class MultiSwitch : public BooleanSwitch
{
public:
	MultiSwitch(int numArgs=-1);           /**< Constructor @param numArgs Number of arguments this switch takes. -1 for infinitive. */
	virtual bool acceptsMoreArguments();
	virtual void giveArgument(std::string);
	void setMaxNumOfArguments(int number); /**< Sets how many arguments this switch takes. -1 for infinitive. */
	unsigned int argumentCount();          /**< @return The amount of arguments that where taken. */
public:
	vector<std::string> arguments;
	int maxNumOfArguments;
};

/** @class SwitchParser
 *  A simple class for parsing command line arguments.
 *  Example usage:
 *  @code
 *  // Declare a switch which takes one argument.
 *  SimpleSwitch swUser;
 *  // Declare a switch which takes two arguments.
 *  MultiSwitch swResolution(2);
 *  // Declares a switch which takes an infinitive amount of switchs.
 *  MultiSwitch swFiles;
 *
 *  SwitchParser parser;
 *
 *  parser.registerSwitch(swUser, "user", 'u');
 *  parser.registerSwitch(swResolution, "resolution", 'r');
 *  parser.registerSwitch(swFiles, "files", 'f');
 *  parser.setAnonymousSwitch(swFiles);
 *
 *  parser.parseArgv(argc, argv);
 *
 *  std::string user = 'root';
 *  if(swUser.isSet)
 *    user = swUser.get<std::string>();
 *  int hgt=800, wdt=600;
 *  if(swResolution.isSet)
 *  {
 *    if(swResolution.argumentCount() != 2)
 *      cout << "Error: -r/--resolution takes exactly 2 arguments." << endl;
 *    else
 *    {
 *      hgt = swResolution.get<int>(0);
 *      wdt = swResolution.get<int>(1);
 *    }
 *  }
 *
 *  if(swFiles.argumentCount == 0)
 *  {
 *    cerr << "No files given!" << endl;
 *    return 1;
 *  }
 *
 *  vector<std::string> files=swFiles.getAll();
 *  foreach(std::string file, files)
 *  {
 *    [...]
 *  }
 *  @endcode
 */
}
#endif // CLIARGUMENTINTERPRETER_H
