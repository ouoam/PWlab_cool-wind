/**
 * @file NexVariable.cpp
 *
 * The implementation of class NexText. 
 *
 * @author  huang xiaoming (email:<xiaoming.huang@itead.cc>)
 * @date    2016/9/13
 * @copyright 
 * Copyright (C) 2014-2015 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#include "NexVariable.h"

NexVariable::NexVariable(uint8_t pid, uint8_t cid, const char *name)
    : NexTouch(pid, cid, name)
{
}

NexVariable::NexVariable(uint8_t pid, uint8_t cid, const char *name, const char *pageName)
    : NexTouch(pid, cid, name, pageName)
{
}

uint32_t NexVariable::getValue(uint32_t *number)
{
    String cmd = String("get ");
    cmd += getPageName();
    cmd += ".";
    cmd += getObjName();
    cmd += ".val";
    sendCommand(cmd.c_str());
    return recvRetNumber(number);
}

bool NexVariable::setValue(uint32_t number)
{
    char buf[10] = {0};
    String cmd;

    utoa(number, buf, 10);
    cmd += getPageName();
    cmd += ".";
    cmd += getObjName();
    cmd += ".val=";
    cmd += buf;

    sendCommand(cmd.c_str());
    return recvRetCommandFinished();
}

uint32_t NexVariable::getText(char *buffer, uint32_t len)
{
    String cmd;
    cmd += "get ";
    cmd += getPageName();
    cmd += ".";
    cmd += getObjName();
    cmd += ".txt";
    sendCommand(cmd.c_str());
    return recvRetString(buffer, len);
}

bool NexVariable::setText(const char *buffer)
{
    String cmd;
    cmd += getPageName();
    cmd += ".";
    cmd += getObjName();
    cmd += ".txt=\"";
    cmd += buffer;
    cmd += "\"";
    sendCommand(cmd.c_str());
    return recvRetCommandFinished();
}