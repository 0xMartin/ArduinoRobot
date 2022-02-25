/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-20 12:39:13
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-20 23:27:15
 *
 *  Copyright (C) 2022 Martin Krcma
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/

#include "module.h"

Module::Module(uint8_t _ID, ModuleType _type, void * _robot_core)
{
    this->ID = _ID;
    this->type = _type;
    this->actions_list = NULL;
    this->robot_core = _robot_core;
}

void Module::process()
{
    // zpracuje samotny modul
    this->__process();
    // zpracuje vsechny akce
    ActionNode *node = this->actions_list;
    if (node != NULL)
    {
        do
        {
            if(node->action == NULL) {
                continue;
            }
            // pokud predicate procedura navrati true pak zavola action callback
            if (((ModuleAction*)node->action)->predicate(this))
            {
                // zavola callback akce
                ((ModuleAction*)node->action)->callback(this);
            }
        } while ((node = node->next) != NULL);
    }
}

uint8_t Module::getID() const
{
    return this->ID;
}

void Module::setRobotCore(void * _robot_core) {
    this->robot_core = _robot_core;
}

ModuleType Module::getType() const
{
    return this->type;
}

bool Module::addAction(void * action)
{
    // alokoje pamet pro novy ActionNode
    ActionNode *node = (ActionNode *)malloc(sizeof(ActionNode));
    if (node == NULL)
    {
        return false;
    }

    // nastavy hodnoty v ActionNode
    node->next = NULL;
    node->action = action;

    // prida novy ActionNode na konec linked listu s ActionNodama
    ActionNode *last = this->actions_list;
    if (last == NULL)
    {
        this->actions_list = node;
    }
    else
    {
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = node;
    }

    return true;
}

bool Module::removeAction(void * action)
{
    ActionNode *last = NULL;
    ActionNode *iter = this->actions_list;
    if (iter != NULL)
    {
        do
        {
            if (iter->action == action)
            {
                // relinkuje list
                if (last == NULL)
                {
                    this->actions_list = iter->next;
                }
                else
                {
                    last->next = iter->next;
                }
                // ovolni pamet
                free(iter);
                return true;
            }
            last = iter;
        } while ((iter = iter->next) != NULL);
    }
    return false;
}

ActionNode *Module::getActionList() const
{
    return this->actions_list;
}