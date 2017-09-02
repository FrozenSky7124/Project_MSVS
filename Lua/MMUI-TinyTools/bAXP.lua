--[材质设置]
media_A = {
	flat = "Interface\\Buttons\\WHITE8x8",
	--font = "Fonts\\ARIALN.TTF"
	font = STANDARD_TEXT_FONT
}

--[参数设置]
config_A = {
	enable = 1, --开启/关闭
	spawn = {"BOTTOM", UIParent, "BOTTOM", 0, 2}, --调整神器条位置（修改后两个数字）
	width = 553, --宽度
	height = 17, --高度
	color = {1.0, 0.9, 0, 1} --颜色
}

if config_A.enable ~= 1 then
	return
end

--[数值格式化函数]
local numberize = function(v)
	-- body
	if v <= 9999 then
		return v
	end
	if v >= 1000000 then
		local value = string.format("%.1fm", v/1000000)
		return value
	elseif v >= 1000 then
		local value = string.format("%.1fk", v/1000)
		return value
	end
end

local abar = CreateFrame("frame", "bAXP", UIParent)
abar:SetPoint(unpack(config_A.spawn))
abar:SetFrameStrata("LOW")
abar:SetClampedToScreen(true)
abar:SetMovable(true)
abar:SetUserPlaced(true)
abar:EnableMouse(true)
abar:RegisterForDrag("LeftButton")
abar:SetWidth(config_A.width)
abar:SetHeight(config_A.height)
abar:SetBackdrop({bgFile = media_A.flat, insets = {top = -2, left = -2, bottom = -2, right = -2}})
abar:SetBackdropColor(0,0,0,1)
abar:SetAlpha(0.5)
--abar:Hide()

axpbar = CreateFrame('StatusBar', nil, abar)
axpbar:SetAllPoints(abar)
axpbar:SetStatusBarTexture(media_A.flat)
axpbar:SetWidth(config_A.width)
axpbar:SetHeight(config_A.height)
axpbar:SetValue(0)
axpbar:SetAlpha(0.5)

abar.bg = abar:CreateTexture("bg", 'BORDER')
abar.bg:SetAllPoints(abar)
abar.bg:SetTexture(media_A.flat)
abar.bg:SetVertexColor(0.16, 0.16, 0.16, 1)

axpbar.text = axpbar:CreateFontString("AXP Text")
axpbar.text:SetPoint("TOPLEFT", abar, "BOTTOMLEFT", 50, 16)
axpbar.text:SetFont(media_A.font, 15, "OUTLINE")
axpbar.text:SetAlpha(0)

--[注册事件]
abar:SetScript("OnDragStart", function(self) if IsShiftKeyDown() then abar:StartMoving() end end)
abar:SetScript("OnDragStop", function(self) abar:StopMovingOrSizing() end)

abar:SetScript("OnEnter", function() axpbar.text:SetAlpha(1) axpbar:SetAlpha(1) abar:SetAlpha(1) end)
abar:SetScript("OnLeave", function() axpbar.text:SetAlpha(0) axpbar:SetAlpha(0.5) abar:SetAlpha(0.5) end)

abar:RegisterEvent("PLAYER_ENTERING_WORLD")
abar:RegisterEvent("ARTIFACT_XP_UPDATE")
abar:RegisterEvent("UNIT_INVENTORY_CHANGED")

--[响应函数]
abar:SetScript("OnEvent", function()
	local itemID, altItemID, name, _, xp, pointsSpent, _, _, _, _, _, _, artifactTier = C_ArtifactUI.GetEquippedArtifactInfo();
	local showArtifact = itemID;
	if showArtifact then
		--print("showArtifact = ", showArtifact)
		abar:Show()
		local xpForNextPoint = C_ArtifactUI.GetCostForPointAtRank(pointsSpent, artifactTier);
		axpbar:SetMinMaxValues(0, xpForNextPoint)
		--当前神器能量大于解锁天赋需求的神器能量
		if (xp > xpForNextPoint) then 
			xp = xpForNextPoint
		end
		axpbar:SetStatusBarColor(unpack(config_A.color))
		axpbar:SetValue(xp)
		axpbar.text:SetText("Lv "..pointsSpent.."   |   "..numberize(xp).." / "..numberize(xpForNextPoint).."   |   "..floor((xp/xpForNextPoint)*1000)/10 .."%".."   |   "..name)
	else
		--print("showArtifact = ", showArtifact)
		abar:Hide()
	end
end)

--[测试] 20170901

SLASH_TOGGLEGRID2 = "/mbaxp"
SlashCmdList["TOGGLEGRID"] = function()
	print("MMUI-TestMode")

	--Artifact Test
	local itemID, altItemID, name, icon, xp, pointsSpent, quality, artifactAppearanceID, appearanceModID, itemAppearanceID, altItemAppearanceID, altOnTop, artifactTier = C_ArtifactUI.GetEquippedArtifactInfo(); --7.3.0 更新了此函数的返回值列表
	local xpForNextPoint = C_ArtifactUI.GetCostForPointAtRank(pointsSpent, artifactTier);  --7.3.0 更新了此函数的参数，新增第二参数：artifactTier，即神器层级。

	print("name = ", name)
	print("xp = ", xp)
	print("pointsSpent = ", pointsSpent)
	print("artifactTier = ", artifactTier)
	print("xpForNextPoint = ", xpForNextPoint)

end


--[[
SLASH_TOGGLEGRID2 = "/mbaxp"
SlashCmdList["TOGGLEGRID"] = function()
	print("MMUI-TestMode")

	--Artifact Test
	local itemID, altItemID, name, icon, xp, pointsSpent, quality, _, _, _, _, altOnTop, artifactMaxed = C_ArtifactUI.GetEquippedArtifactInfo()
	local pointCost = C_ArtifactUI.GetCostForPointAtRank(pointsSpent)
	--local knowledgeLevel = C_ArtifactUI.GetArtifactKnowledgeLevel()
	print("itemID = ", itemID)
	print("altItemID = ", altItemID)
	print("name = ", name)
	print("xp = ", xp)
	print("pointsSpent = ", pointsSpent)
	--print("quality = ", quality)
	--print("altOnTop = ", altOnTop)
	print("artifactMaxed = ", artifactMaxed)
	print("Cost = ", pointCost)
	--print("KnowledgeLevel = ", knowledgeLevel)
	SendChatMessage("MMUI-ArtifactInfo ItemID:"..itemID.." Name:"..name.." CurLevel:"..pointsSpent.." CurXP:"..xp.."/"..pointCost, "SAY")
	--SendChatMessage("MMUI-ArtifactInfo Name:"..name.." CurLevel:"..pointsSpent.." CurXP:"..xp, "OFFICER")
end
--]]

--[[
SendChatMessage("msg", ["chatType"], ["language"], ["channel"])

"SAY" 相当于/s
"EMOTE" 相当于/me
"YELL" 相当于/y
"PARTY" 相当于/p
"GUILD" 相当于/g
"OFFICER" 相当于/o
"RAID" 相当于/ra
"RAID_WARNING" 相当于/rw
"BATTLEGROUND" 相当于/bg
"WHISPER" 相当于/w
]]

--[[  

--因7.3.0更新，以下函数已失效。

local itemID, altItemID, name, icon, xp, pointsSpent, quality, artifactAppearanceID, appearanceModID, itemAppearanceID, altItemAppearanceID, altOnTop, artifactMaxed = C_ArtifactUI.GetEquippedArtifactInfo();

C_ArtifactUI.GetCostForPointAtRank(pointsSpent)

C_ArtifactUI.GetArtifactKnowledgeLevel() - return number of knowledge level

]]