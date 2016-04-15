-- by EUI

local _, sakaras = ...
--LibStub("AceTimer-3.0"):Embed(sakaras)
local upper = string.upper

E = {
	SendDefault = true,				-- 只自己能看到	
	SendChat = false,				-- 发送队伍频道 (宴席,餐桌,糖,召唤门,传送门,机器人,玩具)
	SendGuild = false,				-- 发送在公会频道 (随机奖励包和冬拥,托巴拉德)
	
	HideCombat = true,				-- 开关 进战, 脱战
	--lfgnoti = true,					-- 开关 随机奖励包
	wgtimenoti = false,				-- 开关 冬拥湖, 托巴拉德
	Feasts = true,					-- 开关 大餐, 宴席, 法师召唤餐桌
	Soulwell = true,				-- 开关 灵魂之井(术士糖锅)
	SummonRitual = true,			-- 开关 召唤仪式(术士拉人的门)
	Portals = true,					-- 开关 法师传送门, 烈酒遥控器
	Bots = true,					-- 开关 修理机器人, 布林顿, 随身邮箱
	Toys = false,					-- 开关 玩具火车, 火鸡的羽毛, 自动跳舞信号发射器(跳舞手雷)
	
	Interrupts = true,				-- 当你成功打断的后 进行通告(只发送 队伍 或  SAY 频道)
	
	FlaskFood = false,				-- 通告 合剂,食物 buff(只发送 队伍 或  SAY 频道) 命令: /hj 或 /合剂
	FlaskFoodAuto = false,			-- 当团队就位确认时,进行合剂和食物buff的通告
	ItemsPrompt = false,			-- 进副本提醒是否需要更换装备.
	
	Epos = {"CENTER",UIParent,0,50},	-- 动态提示位置
	Fontsize = 30,						-- 动态提示字号
}

IDtable = {
	Portals = {
		-- 联盟
		[10059] = true,		-- 暴风城
		[11416] = true,		-- 铁炉堡
		[11419] = true,		-- 达纳苏斯
		[32266] = true,		-- 埃索达
		[49360] = true,		-- 塞拉摩
		[33691] = true,		-- 沙塔斯
		[88345] = true,		-- 托尔巴拉德
		[132620] = true,	-- 锦绣谷
		[176246] = true,	-- 暴风之盾
		-- 部落
		[11417] = true,		-- 奥格瑞玛
		[11420] = true,		-- 雷霆崖
		[11418] = true,		-- 幽暗城
		[32267] = true,		-- 银月城
		[49361] = true,		-- 斯通纳德
		[35717] = true,		-- 沙塔斯
		[88346] = true,		-- 托尔巴拉德
		[132626] = true,	-- 锦绣谷
		[176244] = true,	-- 战争之矛
		-- 共同
		[53142] = true,		-- 达拉然
		[120146] = true,	-- 远古达拉然
		[49844] = true,		-- 烈酒遥控器
	},
	Bots = {
		[22700] = true,		-- 修理机器人74A型
		[44389] = true,		-- 战地修理机器人110G
		[54711] = true,		-- 废物贩卖机器人
		[67826] = true,		-- 基维斯
		[126459] = true,	-- 布林顿4000
		[161414] = true,	-- 布林顿5000
	},
	Flask = {
		-- 5.0
		105694,				-- 大地 [耐力]
		105691,				-- 暖阳 [智力]
		105696,				-- 冬噬 [力量]
		105693,				-- 秋叶 [精神]
		105689,				-- 春华 [敏捷]
		-- 6.0
		156071,				-- 德拉诺力量合剂
		156077,				-- 德拉诺耐力合剂
		156070,				-- 德拉诺智力合剂
		156073,				-- 德拉诺敏捷合剂
		156080,				-- 德拉诺强效力量合剂
		156084,				-- 德拉诺强效耐力合剂
		156079,				-- 德拉诺强效智力合剂
		156064,				-- 德拉诺强效敏捷合剂
	},
	Food = {
		104280,				-- 充分进食
	},
}

local GetNextChar = function(word,num)
	-- 达到的效果就是除了最后一个特殊token之外所有的特殊token都会带上一个字符一起输出
	local specialTokenPattern = {"|cff%w%w%w%w%w%w", "|r", "|Hspell:%d+", "|h", "|Hitem:%d+",":%d+", ":%-%d+"}
	local isFindSpecialToken = false
	local tempNum = num
	-- repeat直到找不到特殊token为止 记录下所有特殊token的长度
	-- 这会将|r|cff123456wtf这样的内容中连续的特殊token一次性找出
	repeat
		isFindSpecialToken = false
		for _, v in pairs(specialTokenPattern) do
			local startIndex, endIndex = word:find(v, tempNum)
			if startIndex == tempNum then
				tempNum = tempNum + (endIndex - startIndex + 1)
				isFindSpecialToken = true
				break
			end
		end
	until not isFindSpecialToken
		local tokenLength = tempNum - num
	local c = word:byte(num + tokenLength)
	local shift
	
	if not c then
		if tokenLength > 0 then
			-- 兼容token是字符串末尾的情况 输出token本身
			-- 不过个人认为字符串末尾的token即使不输出也没有问题 未测试
			return word:sub(num, num + tokenLength - 1), (num + tokenLength)
		else
			return "",num
		end
	end
	
	if (c > 0 and c <= 127) then
		shift = 1
	elseif (c >= 192 and c <= 223) then
		shift = 2
	elseif (c >= 224 and c <= 239) then
		shift = 3
	elseif (c >= 240 and c <= 247) then
		shift = 4
	end
	
	shift = shift + tokenLength
	return word:sub(num,num+shift-1),(num+shift)
end

local updaterun = CreateFrame("Frame")

local flowingframe = CreateFrame("Frame",nil,UIParent)
	flowingframe:SetFrameStrata("HIGH")
	flowingframe:SetPoint(unpack(E.Epos))
	flowingframe:SetHeight(64)
	flowingframe:SetScript("OnUpdate", FadingFrame_OnUpdate)
	flowingframe:Hide()
	flowingframe.fadeInTime = 0
	flowingframe.holdTime = 3
	flowingframe.fadeOutTime = 0.5
	
local flowingtext = flowingframe:CreateFontString(nil,"OVERLAY")
	flowingtext:SetPoint("Left")
	flowingtext:SetFont(GameFontNormal:GetFont(), E.Fontsize, 'OUTLINE')	-- 字体
	flowingtext:SetShadowOffset(0,0)
	flowingtext:SetJustifyH("LEFT")
	
local rightchar = flowingframe:CreateFontString(nil,"OVERLAY")
	rightchar:SetPoint("LEFT",flowingtext,"RIGHT")
	rightchar:SetFont(GameFontNormal:GetFont(), E.Fontsize+20, 'OUTLINE')		-- 动态效果字体(大)
	rightchar:SetShadowOffset(0,0)
	rightchar:SetJustifyH("LEFT")

local count,len,step,word,stringE,a

local speed = .03333

local nextstep = function()
	a,step = GetNextChar (word,step)
	flowingtext:SetText(stringE)
	stringE = stringE..a
	rightchar:SetText(a)
end

local updatestring = function(self,t)
	count = count - t
	if count < 0 then
		if step > len then 
			self:Hide()
			flowingtext:SetText(stringE)
			FadingFrame_Show(flowingframe)
			rightchar:SetText("")
			word = ""
		else 
			nextstep()
			FadingFrame_Show(flowingframe)
			count = speed
		end
	end
end

updaterun:SetScript("OnUpdate",updatestring)
updaterun:Hide()

EuiAlertRun = function(f,r,g,b)
	flowingframe:Hide()
	updaterun:Hide()
	
		flowingtext:SetText(f)
		local l = flowingtext:GetWidth()
		
	local color1 = r or 1
	local color2 = g or 1
	local color3 = b or 1
	
	flowingtext:SetTextColor(color1*.95,color2*.95,color3*.95)
	rightchar:SetTextColor(color1,color2,color3)
	
	word = f
	len = f:len()
	step = 1
	count = speed
	stringE = ""
	a = ""
	
		flowingtext:SetText("")
		flowingframe:SetWidth(l)
		
	rightchar:SetText("")
	FadingFrame_Show(flowingframe)
	updaterun:Show()
end

CheckChat = function(warning)
	if (not IsInGroup(LE_PARTY_CATEGORY_HOME) or not IsInRaid(LE_PARTY_CATEGORY_HOME)) and IsInGroup(LE_PARTY_CATEGORY_INSTANCE) then
		return "INSTANCE_CHAT"
	elseif IsInRaid(LE_PARTY_CATEGORY_HOME) then
		if warning and (UnitIsGroupLeader("player") or UnitIsGroupAssistant("player") or IsEveryoneAssistant()) then
			return "RAID_WARNING"
		else
			return "RAID"
		end
	elseif IsInGroup(LE_PARTY_CATEGORY_HOME) then
		return "PARTY"
	end
	return "SAY"
end

local CombatNotification = CreateFrame ("Frame")
local L = {}
local _,localizedName1 = GetWorldPVPAreaInfo(1)
local _,localizedName2 = GetWorldPVPAreaInfo(2)

if(GetLocale()=="zhCN") then
	L.INFO_WOWTIME_TIP = "即将在15分钟内开始"
	L.need = " 有奖励包包了!"
	L.Tank = "|cffEE3A8C<坦克>|r"
	L.Healer = "|cff009300<治疗>|r"
	L.DPS = "|cffEE3A8C<输出>|r"
	L.combat_enter = "进入战斗"
	L.combat_leav = "离开战斗"
				L.Tank_G = "<坦克>"
				L.Healer_G = "<治疗>"
				L.DPS_G = "<输出>"
	L.STAT = "<%s> 放置了 %s - [%s]."
	L.PRE = "<%s> 放置了 %s"
	L.PUT = "<%s> 放置了 %s"
	L.CAST = "<%s> 开启了 %s"
	L.CLICK = "<%s> 正在开启 %s... 请点击！"
	L.USE = "<%s> 使用了 %s."
	L.NOFOOD = "缺少食物Buff: "
	L.NOFLASK = "缺少合剂: "
	L.ALLBUFFED = "已获得所有增益!"
elseif (GetLocale()=="zhTW") then
	L.INFO_WOWTIME_TIP = "即將在15分鐘內開始"
	L.need = " 有獎勵包包了!"
	L.Tank = "|cffA52A00<坦克>|r"
	L.Healer = "|cff009300<治療>|r"
	L.DPS = "|cffEE3A8C<輸出>|r"
	L.combat_enter = "進入戰鬥"
	L.combat_leav = "離開戰鬥"	
				L.Tank_G = "<坦克>"
				L.Healer_G = "<治療>"
				L.DPS_G = "<輸出>"
	L.STAT = "<%s> 放置了 %s - [%s]."
	L.PRE = "<%s> 放置了 %s"
	L.PUT = "<%s> 放置了 %s"
	L.CAST = "<%s> 開啟了 %s"
	L.CLICK = "<%s> 正在開啟 %s... 請點擊！"
	L.USE = "<%s> 使用了 %s."
	L.NOFOOD = "缺少食物Buff: "
	L.NOFLASK = "缺少精煉藥劑: "
	L.ALLBUFFED = "已獲得所有增益!"
elseif (GetLocale()=="enUS") then
	L.INFO_WOWTIME_TIP = "will start within 15 minute"
	L.need = " reward bags!"
	L.Tank = "|cffA52A00<Tank>|r"
	L.Healer = "|cff009300<Healer>|r"
	L.DPS = "|cffEE3A8C<DPS>|r"
	L.combat_enter = "ENTERING COMBAT"
	L.combat_leav = "LEAVING COMBAT"
				L.Tank_G = "<Tank>"
				L.Healer_G = "<Healer>"
				L.DPS_G = "<DPS>"
	L.STAT = "<%s> has prepared a %s - [%s]."
	L.PRE = "<%s> has prepared a %s."
	L.PUT = "<%s> has put down a %s."
	L.CAST = "<%s> is casting %s."
	L.CLICK = "<%s> is casting %s. Click!"
	L.USE = "<%s> used a %s."
	L.NOFOOD = "No Food: "
	L.NOFLASK = "No Flask: "
	L.ALLBUFFED = "All Buffed!"
end

-- 进战/脱战
if E.HideCombat == true then
	SetCVar("fctCombatState", 0)
	CombatNotification:RegisterEvent("PLAYER_REGEN_ENABLED")
	CombatNotification:RegisterEvent("PLAYER_REGEN_DISABLED")
	CombatNotification:SetScript("OnEvent", function (self,event)
		if (UnitIsDead("player")) then return end
		if event == "PLAYER_REGEN_ENABLED" then
			EuiAlertRun(L.combat_leav,0.1,1,0.1)
		elseif event == "PLAYER_REGEN_DISABLED" then
			EuiAlertRun(L.combat_enter,1,0.1,0.1)
		end	
	end)
end

-- 托尔巴拉德, 冬拥湖 15 分钟开始
local int = 1
local clocks_update = function(self,t)
	if E.wgtimenoti ~= true then return end
	int = int - t
	if int > 0 then return end
	local INFO_WOWTIME_TIP = L.INFO_WOWTIME_TIP
	
	int = 1
	local _,_,_,canQueue1,wgtime1 = GetWorldPVPAreaInfo(1)
	local _,_,_,canQueue2,wgtime2 = GetWorldPVPAreaInfo(2)
	local canSend = IsInRaid() and IsInInstance()
	local isActive = 0
	if canQueue1 == false and wgtime1 == 900 then
		isActive = 1;
		INFO_WOWTIME_TIP = localizedName1.. INFO_WOWTIME_TIP
	end
	if canQueue2 == false and wgtime2 == 900 then
		isActive = 2;
		INFO_WOWTIME_TIP = localizedName2.. INFO_WOWTIME_TIP
	end
	if isActive > 0 then
		if not canSend then
			EuiAlertRun (INFO_WOWTIME_TIP)
			if E.SendGuild then SendChatMessage('=>·'.. INFO_WOWTIME_TIP, "GUILD", nil, nil) end
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage('|cffff3333=>|r·'.. INFO_WOWTIME_TIP,1,1,1) end
		end
	end
end
CombatNotification:SetScript("OnUpdate",clocks_update)


--[=[ 5H随机地下城奖励
local f = CreateFrame("Frame")
f:RegisterEvent("ZONE_CHANGED_NEW_AREA")

function f:lfgMsg()
	local str, strg = '',''
	if E.lfgnoti ~= true then return end
	for i = 1, GetNumDungeons() do
		local _, name = GetLFGDungeonInfo(i)
	end
	
	if not name then name = ''; end
	local canSend = IsInRaid() and IsInInstance()
	local eligible, forTank, forHealer, forDamage, itemCount, money, xp = GetLFGRoleShortageRewards(462, 1)
	if eligible and (itemCount > 0) then
		if forTank then 
			str = str.. L.Tank 
			strg = strg.. L.Tank_G
		end
		if forHealer then 
			str = str.. L.Healer 
			strg = strg.. L.Healer_G
		end
		if forDamage then 
			str = str.. L.DPS 
			strg = strg.. L.DPS_G
		end
		if not canSend then
			EuiAlertRun(name.. str.. L.need)
			if E.SendGuild then SendChatMessage('=>·'.. name.. strg.. L.need, "GUILD", nil, nil) end
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage('|cffff3333=>|r·'.. name.. str.. L.need,1,1,1) end
		end
		str, strg = "",""
		sakaras:ScheduleTimer(f.lfgMsg, random(300, 600))
	else
		sakaras:ScheduleTimer(f.lfgMsg, random(1, 10))
	end
end
sakaras:ScheduleTimer(f.lfgMsg, 10);
--]=]

-----------
-- 扩展
-----------
local Announces = CreateFrame("Frame")
Announces:RegisterEvent("COMBAT_LOG_EVENT_UNFILTERED")
Announces:SetScript("OnEvent", function(self, event, _, subEvent, _, _, srcName, _, _, _, destName, _, _, spellID, ...)
	if not IsInGroup() or InCombatLockdown() or not subEvent or not spellID or not srcName then return end
	if not UnitInRaid(srcName) and not UnitInParty(srcName) then return end

	local srcName = format(srcName:gsub("%-[^|]+", ""))
	if subEvent == "SPELL_CAST_SUCCESS" then
		-- 烧烤 [力量]
		if E.Feasts and (spellID == 126492 or spellID == 126494) then
			EuiAlertRun(string.format(L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT1_NAME))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT1_NAME)) end
			if E.SendChat then SendChatMessage(string.format(L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT1_NAME), CheckChat(true)) end
		-- 烹炒 [敏捷]
		elseif E.Feasts and (spellID == 126495 or spellID == 126496) then
			EuiAlertRun(string.format(L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT2_NAME))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT2_NAME)) end
			if E.SendChat then SendChatMessage(string.format(L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT2_NAME), CheckChat(true)) end
		-- 烘焙 [耐力]
		elseif E.Feasts and (spellID == 126501 or spellID == 126502) then
			EuiAlertRun(string.format(L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT3_NAME))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT3_NAME)) end
			if E.SendChat then SendChatMessage(string.format(L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT3_NAME), CheckChat(true)) end
		-- 炖煮 [智力]
		elseif E.Feasts and (spellID == 126497 or spellID == 126498) then
			EuiAlertRun(string.format(L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT4_NAME))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT4_NAME)) end
			if E.SendChat then SendChatMessage(string.format(L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT4_NAME), CheckChat(true)) end
		-- 蒸烧 [精神]
		elseif E.Feasts and (spellID == 126499 or spellID == 126500) then
			EuiAlertRun(string.format(L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT5_NAME))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT5_NAME)) end
			if E.SendChat then SendChatMessage(string.format(L.STAT, srcName, GetSpellLink(spellID), SPELL_STAT5_NAME), CheckChat(true)) end
		-- 熊猫人, 酿造
		elseif E.Feasts and (spellID == 104958 or spellID == 105193 or spellID == 126503 or spellID == 126504 ) then
			EuiAlertRun(string.format(L.PRE, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.PRE, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.PRE, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		-- 5.4 餐车
		elseif E.Feasts and (spellID == 145166 or spellID == 145169 or spellID == 145196) then
			EuiAlertRun(string.format(L.PRE, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.PRE, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.PRE, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		-- 6.0 德拉诺: 千水鱼宴, 红肉盛宴, 狂野大餐, 
		elseif E.Feasts and (spellID == 160914 or spellID == 160740 or spellID == 175215) then
			EuiAlertRun(string.format(L.PRE, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.PRE, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.PRE, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		-- 法师召唤餐桌
		elseif E.Feasts and spellID == 43987 then			
			EuiAlertRun(string.format(L.PRE, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.PRE, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.PRE, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		-- 召唤仪式
		elseif E.SummonRitual and spellID == 698 then		
			EuiAlertRun(string.format(L.CLICK, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.CLICK, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.CLICK, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		end
		
	elseif subEvent == "SPELL_SUMMON" then
		-- 修理机器人, 布林顿4000/5000
		if E.Bots and IDtable.Bots[spellID] then
			EuiAlertRun(string.format(L.PUT, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.PUT, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.PUT, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		end
		
	elseif subEvent == "SPELL_CREATE" then
		-- 灵魂之井
		if E.Soulwell and spellID == 29893 then
			EuiAlertRun(string.format(L.PUT, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.PUT, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.PUT, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		-- 随身邮箱
		elseif E.Bots and spellID == 54710 then
			EuiAlertRun(string.format(L.PUT, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.PUT, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.PUT, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		-- 玩具火车
		elseif E.toys and spellID == 61031 then
			EuiAlertRun(string.format(L.PUT, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.PUT, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.PUT, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		-- 法师传送门,烈酒遥控器
		elseif E.Portals and IDtable.Portals[spellID] then
			EuiAlertRun(string.format(L.CAST, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.CAST, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.CAST, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		end
		
	elseif subEvent == "SPELL_AURA_APPLIED" then
		-- 火鸡的羽毛, 自动跳舞信号发射器
		if E.Toys and (spellID == 61781 or ((spellID == 51508 or spellID == 51510) and destName == select(1, UnitName("player")))) then		
			EuiAlertRun(string.format(L.USE, srcName, GetSpellLink(spellID)))
			if E.SendDefault then DEFAULT_CHAT_FRAME:AddMessage(string.format('|cffff3333=>|r·'.. L.USE, srcName, GetSpellLink(spellID))) end
			if E.SendChat then SendChatMessage(string.format(L.USE, srcName, GetSpellLink(spellID)), CheckChat(true)) end
		end
	end
end)


--	当你成功打断的时候在队伍/团队中进行通告
if E.Interrupts == true then
	local interrupts = CreateFrame("Frame")
	interrupts:RegisterEvent("COMBAT_LOG_EVENT_UNFILTERED")
	interrupts:SetScript("OnEvent", function(self, _, ...)
		local _, event, _, sourceGUID, _, _, _, _, destName, _, _, _, _, _, spellID = ...
		if not (event == "SPELL_INTERRUPT" and sourceGUID == UnitGUID("player")) then return end
		SendChatMessage(INTERRUPTED.." "..destName..": "..GetSpellLink(spellID), CheckChat())
	end)
end


--	检查合剂和食物 buff
if E.FlaskFood == true then
	local noFood, noFlask, unitBuffs = {}, {}, {}
	local foods = IDtable["Food"]
	local flasks = IDtable["Flask"]

	local function scan(unit)
		table.wipe(unitBuffs)
		local i = 1
		while true do
			local name = UnitAura(unit, i, "HELPFUL")
			if not name then return end
			unitBuffs[name] = true
			i = i + 1
		end
	end

	local function checkFlask(unit)
		scan(unit)
		for _, id in pairs(flasks) do
			if unitBuffs[GetSpellInfo(id)] then
				return true
			end
		end
	end

	local function checkFood(unit)
		scan(unit)
		for _, id in pairs(foods) do
			if unitBuffs[GetSpellInfo(id)] then
				return true
			end
		end
	end

	local function checkUnit(unit)
		local name = UnitName(unit)
		if not checkFood(unit) then
			noFood[#noFood + 1] = name
		end
		if not checkFlask(unit) then
			noFlask[#noFlask + 1] = name
		end
	end

	local function run(autoreport)
		local checkType
		local output

		table.wipe(noFood)
		table.wipe(noFlask)

		if UnitInRaid("player") then
			checkType = "raid"
		else
			checkType = "party"
			checkUnit("player")
		end

		for i = 1, GetNumGroupMembers() do
			if checkType == "raid" then
				local online = select(8, GetRaidRosterInfo(i))
				if online then
					local unit = checkType..i
					checkUnit(unit)
				end
			else
				local unit = checkType..i
				if UnitIsConnected(unit) then
					checkUnit(unit)
				end
			end
		end

		if #noFlask > 0 then
			table.sort(noFlask)
			output = L.NOFLASK..table.concat(noFlask, ", ")
			SendChatMessage(output, CheckChat())
		end

		if #noFood > 0 then
			table.sort(noFood)
			output = L.NOFOOD..table.concat(noFood, ", ")
			SendChatMessage(output, CheckChat())
		end

		if #noFood == 0 and #noFlask == 0 then
			SendChatMessage(L.ALLBUFFED, CheckChat())
		end
	end

	local frame = CreateFrame("Frame")
	frame:RegisterEvent("READY_CHECK")
	frame:SetScript("OnEvent", function(self, event, ...)
		if E.FlaskFoodAuto then
			run(true)
		end
	end)

	SlashCmdList.FFCHECK = run
	SLASH_FFCHECK1 = "/ffcheck"
	SLASH_FFCHECK2 = "/hj"
	SLASH_FFCHECK3 = "/合剂"
end



-- 提醒换装
if E.ItemsPrompt == true then
	local framenoss = CreateFrame("Frame")
	framenoss:RegisterEvent("ZONE_CHANGED_NEW_AREA")
	framenoss:SetScript("OnEvent", function(self, event)
		if event ~= "ZONE_CHANGED_NEW_AREA" or not IsInInstance() then return end;
		local spec = GetSpecialization();
		if not spec then return NONE, NONE end;
		local _, TalentName = GetSpecializationInfo(spec);
		EuiAlertRun("你现在的天赋是 |cffEE3A8C(".. TalentName.. ")|r, 请检查|cff00FF00装备|r是否符合", 0, 1, 1);
		DEFAULT_CHAT_FRAME:AddMessage(string.format("你现在的天赋是 |cffEE3A8C(".. TalentName.. ")|r, 请检查|cff00FF00装备|r是否符合."), 0, 1, 1);
	end)
end