-- 显示法术ID和来源：取自SpellID插件并改进
local fontcolor = { r=0.44, g=0.83, b=1 }
local SPELLID, FROM

if GetLocale() == "zhCN" then
	SPELLID = "法术编号："
	FROM = "源自："
elseif GetLocale() == "zhTW" then
	SPELLID = "法術編號："
	FROM = "源自："
else
	SPELLID = "SpellID: "
	FROM = " from: "
end

local function UnitToColorText(unit)
	local ClassColor = RAID_CLASS_COLORS[select(2,UnitClass(unit))] or fontcolor;
	if (not UnitIsPlayer(unit)) then ClassColor = fontcolor end;
	return format("|cFF%s%s|r",format("%02X%02X%02X",ClassColor.r*255,ClassColor.g*255,ClassColor.b*255),UnitName(unit));
end

hooksecurefunc(GameTooltip, "SetUnitBuff", function(self,...)
	local _,_,_,_,_,_,_,unitCaster,_,_,id = UnitBuff(...);
	if id then
		if unitCaster then
			self:AddDoubleLine("|cFF00FF00"..SPELLID.."|cFF71D5FF"..id.."|r","|cFF00FF00Buff"..FROM..UnitToColorText(unitCaster));
		else
			self:AddLine("|cFF00FF00"..SPELLID.."|cFF71D5FF"..id.."|r");
		end
		self:Show();
	end
end)

hooksecurefunc(GameTooltip, "SetUnitDebuff", function(self,...)
	local _,_,_,_,_,_,_,unitCaster,_,_,id = UnitDebuff(...);
	if id then
		if unitCaster then
			self:AddDoubleLine("|cFF00FF00"..SPELLID.."|cFF71D5FF"..id.."|r","|cFF00FF00Debuff"..FROM..UnitToColorText(unitCaster));
		else
			self:AddLine("|cFF00FF00"..SPELLID.."|cFF71D5FF"..id.."|r");
		end
		self:Show();
	end
end)

hooksecurefunc(GameTooltip, "SetUnitAura", function(self,...)
	local _,_,_,_,_,_,_,unitCaster,_,_,id = UnitAura(...);
	if id then
		if unitCaster then
			self:AddDoubleLine("|cFF00FF00"..SPELLID.."|cFF71D5FF"..id.."|r","|cFF00FF00Aura"..FROM..UnitToColorText(unitCaster));
		else
			self:AddLine("|cFF00FF00"..SPELLID.."|cFF71D5FF"..id.."|r");
		end
		self:Show();
	end
end)