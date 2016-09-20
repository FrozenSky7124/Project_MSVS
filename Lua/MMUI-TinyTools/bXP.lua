media = {
	flat = "Interface\\Buttons\\WHITE8x8",
	font = "Interface\\Addons\\MMUI-TinyTools\\Media\\font.ttf"
}
config = {
	enable = 1,
	spawn = {"BOTTOM", UIParent, "BOTTOM", 0, 100},
	width = 800,
	height = 23,
	color = {.4, .1, 0.6, 1},
	restedcolor = {.2, .4, 0.8, 1}
}

if config.enable ~= 1 then
return end

local bar = CreateFrame("frame", "bXP", UIParent)
bar:SetPoint(unpack(config.spawn))
bar:SetFrameStrata("LOW")
bar:SetClampedToScreen(true)
bar:SetMovable(true)
bar:SetUserPlaced(true)
bar:EnableMouse(true)
bar:RegisterForDrag("LeftButton")
bar:SetWidth(config.width)
bar:SetHeight(config.height)
bar:SetBackdrop({bgFile = media.flat, insets = {top = -2, left = -2, bottom = -2, right = -2}})
bar:SetBackdropColor(0,0,0,1)
bar:Hide()

xpbar = CreateFrame('StatusBar', nil, bar)
xpbar:SetAllPoints(bar)
xpbar:SetStatusBarTexture(media.flat)
xpbar:SetWidth(config.width)
xpbar:SetHeight(config.height)
xpbar:SetValue(0)

rxpbar = CreateFrame('StatusBar', nil, bar)
rxpbar:SetAllPoints(bar)
rxpbar:SetStatusBarTexture(media.flat)
rxpbar:SetWidth(config.width)
rxpbar:SetHeight(config.height)
rxpbar:SetValue(0)
rxpbar:SetStatusBarColor(unpack(config.restedcolor))
rxpbar:SetAlpha(0.4)
rxpbar:Hide()

bar:SetScript("OnDragStart", function(self) if IsShiftKeyDown() then bar:StartMoving() end end)
bar:SetScript("OnDragStop", function(self) bar:StopMovingOrSizing() end)
--bar:SetScript("OnEnter", function() xpbar.text:SetAlpha(1) end)
bar:SetScript("OnEnter", function() xpbar.text:SetAlpha(1) bar:SetAlpha(1) end)
bar:SetScript("OnLeave", function() xpbar.text:SetAlpha(0) bar:SetAlpha(0) end)
--MakeMovable(bar)

local numberize = function(v)
	if v <= 9999 then return v end
	if v >= 1000000 then
		local value = string.format("%.1fm", v/1000000)
		return value
	elseif v >= 1000 then
		local value = string.format("%.1fk", v/1000)
		return value
	end
end

bar.bg = bar:CreateTexture("bg", 'BORDER')
bar.bg:SetAllPoints(bar)
bar.bg:SetTexture(media.flat)
bar.bg:SetVertexColor(0.16,0.16,0.16,1)
        
xpbar.text = xpbar:CreateFontString("XP Text")
xpbar.text:SetPoint("TOPLEFT", bar, "BOTTOMLEFT", 50, 18)
xpbar.text:SetFont(media.font, 15, "OUTLINE")
xpbar.text:SetAlpha(0)

bar:RegisterEvent("PLAYER_XP_UPDATE")
bar:RegisterEvent("PLAYER_LEVEL_UP")
bar:RegisterEvent("PLAYER_ENTERING_WORLD")
bar:RegisterEvent("UPDATE_EXHAUSTION");
bar:RegisterEvent("UPDATE_FACTION")

bar:SetScript("OnEvent", function()
    xp = UnitXP("player")
    mxp = UnitXPMax("player")
    rxp = GetXPExhaustion("player")
    name, standing, minrep, maxrep, value = GetWatchedFactionInfo()
	
	bar:Show()
	bar:SetAlpha(0)
    xpbar:SetMinMaxValues(0,mxp)
    if UnitLevel("player") == MAX_PLAYER_LEVEL or IsXPUserDisabled == true then
        if name then
            xpbar:SetStatusBarColor(FACTION_BAR_COLORS[standing].r, FACTION_BAR_COLORS[standing].g, FACTION_BAR_COLORS[standing].b, 1)
			xpbar:SetMinMaxValues(minrep,maxrep)
			xpbar:SetValue(value)
			xpbar.text:SetText(value-minrep.." / "..maxrep-minrep.." | "..floor(((value-minrep)/(maxrep-minrep))*1000)/10 .."% | ".. name)
        else
			bar:Hide()
        end
    else
        xpbar:SetStatusBarColor(unpack(config.color))
		xpbar:SetValue(xp)
		if rxp then
			xpbar.text:SetText(numberize(xp).." / "..numberize(mxp).." | "..floor((xp/mxp)*1000)/10 .."%" .. " (+"..numberize(rxp)..")")
			xpbar:SetMinMaxValues(0,mxp)
			rxpbar:SetMinMaxValues(0, mxp)
			xpbar:SetStatusBarColor(unpack(config.restedcolor))
			xpbar:SetValue(xp)
			if (rxp+xp) >= mxp then
				rxpbar:SetValue(mxp)
			else
				rxpbar:SetValue(xp+rxp)
			end
			rxpbar:Show()
		elseif xp then
			xpbar.text:SetText(numberize(xp).." / "..numberize(mxp).." | "..floor((xp/mxp)*1000)/10 .."%")
			rxpbar:Hide()
		end
    end
end)