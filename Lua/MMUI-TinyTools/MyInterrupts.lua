--当你成功打断的时候进行通告

--打断喊话模块
local interrupts = CreateFrame("Frame", nil, UIParent)
interrupts:RegisterEvent("COMBAT_LOG_EVENT_UNFILTERED")
interrupts:SetScript("OnEvent", function (self, _, ... )
	local _, event, _, sourceGUID, _, _, _, _, destName, _, _, _, _, _, spellID = ...
	if event == "SPELL_INTERRUPT" and sourceGUID == UnitGUID("player") then
		SendChatMessage("冻住！不许读条 -> "..GetSpellLink(spellID), "SAY")
	end
end)