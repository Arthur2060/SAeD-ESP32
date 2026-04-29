#include <map>

#pragma once

enum class SAeDStateMap
{
    Wait,
    Mapping,
    Demarc
};

std::map<SAeDStateMap, SAeDStateMap> SAeDTransitionMap = {
    {SAeDStateMap::Wait, SAeDStateMap::Mapping},
    {SAeDStateMap::Mapping, SAeDStateMap::Demarc},
    {SAeDStateMap::Demarc, SAeDStateMap::Wait}
};

enum class SAeDStateDispatch
{
    Wait,
    GetFromStock,
    Dispatch
};

std::map<SAeDStateDispatch, SAeDStateDispatch> SAeDTransitionDispatch = {
    {SAeDStateDispatch::Wait, SAeDStateDispatch::GetFromStock},
    {SAeDStateDispatch::GetFromStock, SAeDStateDispatch::Dispatch},
    {SAeDStateDispatch::Dispatch, SAeDStateDispatch::Wait}
};

enum class SAeDStateNewItem
{
    Wait,
    GetNew,
    Analise,
    Stock
};

std::map<SAeDStateNewItem, SAeDStateNewItem> SAeDTransitionNewItem = {
    {SAeDStateNewItem::Wait, SAeDStateNewItem::GetNew},
    {SAeDStateNewItem::GetNew, SAeDStateNewItem::Analise},
    {SAeDStateNewItem::Analise, SAeDStateNewItem::Stock},
    {SAeDStateNewItem::Stock, SAeDStateNewItem::Wait}
};