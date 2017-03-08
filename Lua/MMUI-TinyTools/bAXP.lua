--[材质设置]
media_A = {
	flat = "Interface\\Buttons\\WHITE8x8",
	--font = "Fonts\\ARIALN.TTF"
	font = STANDARD_TEXT_FONT				--字体设置（可按照上一行修改为指定字体）
}

--[参数设置]
config_A = {
	enable = 1,										--插件开关
	spawn = {"BOTTOM", UIParent, "BOTTOM", 0, 2},	--位置
	width = 553,									--宽度
	height = 17,									--高度
	color = {1.0, 0.9, 0, 1}						--填充颜色
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
	local itemID, _, name, _, xp, curLevel, _, _, _, _, _, _, artifactMaxed = C_ArtifactUI.GetEquippedArtifactInfo()	
	--当不存在神器或者神器能量已满的情况下隐藏能量条
	local showArtifact = itemID and not artifactMaxed;
	if showArtifact then
		--print("showArtifact = ", showArtifact)
		abar:Show()
		local pointCost = C_ArtifactUI.GetCostForPointAtRank(curLevel)
		axpbar:SetMinMaxValues(0, pointCost)
		--当前神器能量大于解锁天赋需求的神器能量
		if (xp > pointCost) then 
			xp = pointCost
		end
		axpbar:SetStatusBarColor(unpack(config_A.color))
		axpbar:SetValue(xp)
		axpbar.text:SetText("Lv "..curLevel.."  |  "..numberize(xp).." / "..numberize(pointCost).."  |  "..floor((xp/pointCost)*1000)/10 .."%".."  |  "..name)
	else
		--print("showArtifact = ", showArtifact)
		abar:Hide()
	end
end)

--[测试]

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
获取当前装备的神器信息
local itemID, altItemID, name, icon, xp, pointsSpent, quality, 
artifactAppearanceID, appearanceModID, itemAppearanceID, altItemAppearanceID, 
altOnTop, artifactMaxed 
= C_ArtifactUI.GetEquippedArtifactInfo();

C_ArtifactUI.GetCostForPointAtRank(pointsSpent)

C_ArtifactUI.GetArtifactKnowledgeLevel() - return number of knowledge level
]]