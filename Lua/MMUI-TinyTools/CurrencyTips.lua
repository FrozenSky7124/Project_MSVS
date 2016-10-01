--常用货币监控

--征服点数 390
--荣誉点数 392
--时空扭曲徽章 1166
--勇气点数 1191
--埃匹希斯水晶 823
--要塞物资 824
--原油 1101
--神器碎片 944
--远古魔力 1155
--职业大厅资源 1220
local F, Currency = CreateFrame("Frame"), {1220, 1155, 824, 1101, 1191}
F.icon = {} 
for i = 1, #Currency do 
local icon = CreateFrame("Frame", nil, UIParent) 
--icon:SetPoint("BOTTOMRIGHT", _G["Minimap"], "BOTTOMLEFT", 3, 20*i+2*(i-1)-10) 
icon:SetPoint("TOPRIGHT", _G["Minimap"], "BOTTOMRIGHT", -2, -25*i+2*(i-1)-20)
--icon:SetPoint("BOTTOMRIGHT", UIParent, "BOTTOMRIGHT", -90*i+2*(i-1)-200, 60) 
icon:SetSize(20, 20) 
icon.texture = icon:CreateTexture(nil, "ARTWORK") 
icon.texture:SetAllPoints(icon) 
icon.texture:SetTexCoord(.1, .9, .1, .9) 

icon:SetBackdrop({ 
   bgFile = "Interface\\AddOns\\MMUI-TinyTools\\Media\\glowTex", -- 背景材质路径 
   insets = {left = 1,right = 1,top = 1,bottom = 1}, -- 背景收缩程度，单位为像素，例如，top = 1即背景材质的上边缘向内收缩1个像素 
   edgeFile = "Interface\\AddOns\\MMUI-TinyTools\\Media\\glowTex", -- 边框材质路径 
   edgeSize = 1, -- 边框宽度 
})
icon:SetBackdropColor(1, 1, 1, 0.6)
icon:SetBackdropBorderColor(0, 0, 0, 1)

icon.text = icon:CreateFontString() 
icon.text:SetPoint("RIGHT", icon, "LEFT", -1, 0) 
--icon.text:SetFont(STANDARD_TEXT_FONT, 18, "NONE") 
icon.text:SetFont(STANDARD_TEXT_FONT, 18, "THINOUTLINE") 
--icon.text:SetShadowOffset(1, 1) 
--icon.text:SetShadowColor(0,0,0,.7) 
F.icon[i] = icon 
end 

local GetCurrencyInfo, select, sub = GetCurrencyInfo, select, string.sub 

for i = 1, #Currency do 
local label, _, icon = GetCurrencyInfo(Currency[i]) 
F.icon[i].texture:SetTexture(icon) 
F.icon[i]:SetScript("OnEnter", function() 
_G["GameTooltip"]:ClearLines() 
_G["GameTooltip"]:SetOwner(F.icon[i], "ANCHOR_LEFT") 
_G["GameTooltip"]:AddLine(sub(label, 1, 6)) 
_G["GameTooltip"]:Show() 
end) 
F.icon[i]:SetScript("OnLeave", function() 
_G["GameTooltip"]:Hide() 
end) 
end 

F:RegisterEvent("CURRENCY_DISPLAY_UPDATE") 
F:SetScript("OnEvent", function() 
for i = 1, #Currency do 
F.icon[i].text:SetText(select(2, GetCurrencyInfo(Currency[i]))) 
end 
end)

--http://bbs.nga.cn/read.php?&tid=8383887