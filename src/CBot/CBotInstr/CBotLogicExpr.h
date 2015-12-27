/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2015, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsitec.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

#pragma once

#include "CBot/CBotInstr/CBotInstr.h"

namespace CBot
{

/*!
 * \brief The CBotLogicExpr class
 */
class CBotLogicExpr : public CBotInstr
{
public:

    /*!
     * \brief CBotLogicExpr
     */
    CBotLogicExpr();

    /*!
     * \brief ~CBotLogicExpr
     */
    ~CBotLogicExpr();

    /*!
     * \brief Execute
     * \param pStack
     * \return
     */
    bool Execute(CBotStack* &pStack) override;

    /*!
     * \brief RestoreState
     * \param pj
     * \param bMain
     */
    void RestoreState(CBotStack* &pj, bool bMain) override;

protected:
    virtual const std::string GetDebugName() { return "CBotLogicExpr"; }
    virtual std::map<std::string, CBotInstr*> GetDebugLinks();

private:
    //! Test to evaluate
    CBotInstr*    m_condition;
    //! Left element
    CBotInstr*    m_op1;
    //! Right element
    CBotInstr*    m_op2;
    friend class CBotTwoOpExpr;
};

} // namespace CBot
