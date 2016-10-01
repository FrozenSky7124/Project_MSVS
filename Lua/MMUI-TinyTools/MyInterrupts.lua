--当你成功打断的时候在队伍/团队中进行通告

--判断是否在队伍中
CheckChat = function()
	if IsInGroup(LE_PARTY_CATEGORY_HOME) then
		return "PARTY"
	end
	return "SAY"
end

--打断喊话模块
local interrupts = CreateFrame("Frame")
interrupts:RegisterEvent("COMBAT_LOG_EVENT_UNFILTERED")
interrupts:SetScript("OnEvent", function (self, _, ... )
	local _, event, _, sourceGUID, _, _, _, _, destName, _, _, _, _, _, spellID = ...
	if not (event == "SPELL_INTERRUPT" and sourceGUID == UnitGUID("player")) then return end
	SendChatMessage(Interrupt.." "..GetSpellLink(spellID), CheckChat())
end)