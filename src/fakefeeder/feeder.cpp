/*
 * Copyright (C) 2012 Jolla Ltd.
 * Contact: Dmitry Rozhkov <dmitry.rozhkov@jollamobile.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "feeder.h"

Feeder::Feeder()
{
    feed = MEventFeed::instance();
}

void Feeder::newfeed()
{
    qlonglong ret;

    ret = feed->addItem(QString("icon-m-content-document"),
                           QString("MasterCard introduces PayPass Wallet Services, Online and API at CTIA 2012"),
                           QString("MasterCard's PayPass is certainly one of the bigger players in the contactless payment scene. But, simply putting a few thousand branded payment terminals in stores across the nation isn't enough. The next step is expanding that empire..."),
                           QStringList("/home/nemo/merlogo.png"),
                           QDateTime::currentDateTime(),
                           QString("by Terrence O'Brien"),
                           false,
                           QUrl("http://www.engadget.com/2012/05/07/mastercard-introduces-paypass-wallet-services-online-and-api-at/"),
                           QString("fakefeeder"),
                           QString("fakefeeder"));
    qDebug() << "Ret" << ret;

    feed->addItem(QString("icon-m-content-document"),
                           QString("Gogo grabs 1MHz spectrum from JetBlue subsidiary LiveTV, beefs up in-flight bandwidth "),
                           QString("In-flight internet provider Gogo's been increasing its domestic presence of late, so it makes sense for the company to snag some additional spectrum as well"),
                           QStringList("/home/nemo/merlogo.png"),
                           QDateTime::currentDateTime(),
                           QString("by Michael Gorman"),
                           true,
                           QUrl("http://www.engadget.com/2012/05/07/gogo-buys-1mhz-spectrum-inflight-wifi/"),
                           QString("fakefeeder"),
                           QString("fakefeeder"));

    feed->addItem(QString("icon-m-content-document"),
                           QString("India launches antitrust investigation against Google"),
                           QString("Google remains tied up in a censorship case with the Indian government, and things aren't looking rosy for the search giant in the interim period before its May 23rd court hearing. The ..."),
                           QStringList(),
                           QDateTime::currentDateTime(),
                           QString("by Sarah Silbert"),
                           false,
                           QUrl("http://www.engadget.com/2012/05/07/india-launches-antitrust-investigation-against-google/"),
                           QString("fakefeeder"),
                           QString("fakefeeder"));

    feed->addItem(QString("icon-m-content-document"),
                           QString("HTC Droid Incredible 4G LTE for Verizon hands-on at CTIA 2012"),
                           QString("This handset didn't surprise anyone when it was officially announced at CTIA Wireless 2012, but we're still happy to spend time with the HTC Droid Incredible 4G LTE for Verizon Wireless regardless. After catching enough glimpses of the smartphone over the past few months ..."),
                           QStringList(),
                           QDateTime::currentDateTime(),
                           QString("by Brad Molen"),
                           false,
                           QUrl("http://www.engadget.com/2012/05/07/htc-droid-incredible-4g-lte-for-verizon-hands-on/"),
                           QString("fakefeeder"),
                           QString("fakefeeder"));

    feed->addItem(QString("icon-m-content-document"),
                           QString("Hands-on with SuperTooth Disco2"),
                           QString("Move over Jambox, there's a new portable audio contender entering the market. The Disco2, shown off at this week's CTIA 2012, is SuperTooth's smaller, more compact sequel to its O.G. 28 watt powerhouse of the same name and a direct threat to the niche market occupied by Jawbone."),
                           QStringList(),
                           QDateTime::currentDateTime(),
                           QString("by Joseph Volpe"),
                           false,
                           QUrl("http://www.engadget.com/2012/05/07/hands-on-with-supertooth-disco2/"),
                           QString("fakefeeder"),
                           QString("fakefeeder"));

    feed->addItem(QString("icon-m-content-document"),
                           QString("Hands-on with Edgewater's WiFi3"),
                           QString("Edgewater may not be a familiar name for even the most tech savvy, but its WiFi3 chipset is poised to propel the wireless industry beyond the limits of traditional access point tech. Aiming to leap past the low power solutions provided by the likes of Cisco"),
                           QStringList(),
                           QDateTime::currentDateTime(),
                           QString("by Joseph Volpe"),
                           false,
                           QUrl("http://www.engadget.com/2012/05/07/hands-on-with-edgewaters-wifi3/"),
                           QString("fakefeeder"),
                           QString("fakefeeder"));

    feed->addItem(QString("icon-m-content-document"),
                           QString("Kyocera teases bone conduction audio technology for mobile phones at CTIA "),
                           QString("Can you hear me now? It's a common phrase in the mobile industry, but if Kyocera has its way, the saying could quickly become a thing of the past, thanks to bone conduction technology that it plans to integrate into future handsets."),
                           QStringList(),
                           QDateTime::currentDateTime(),
                           QString("by Zachary Lutz"),
                           false,
                           QUrl("http://www.engadget.com/2012/05/07/kyocera-teases-bone-conduction-audio-technology-for-mobile-phone/"),
                           QString("fakefeeder"),
                           QString("fakefeeder"));

    feed->addItem(QString("icon-m-content-document"),
                           QString("Samsung, Qualcomm start up Alliance for Wireless Power to take on Qi"),
                           QString("Wireless power has until now been closely associated with the Wireless Power Consortium's Qi standard. There's now a second proposed common ground in the Alliance for Wireless Power, or A4WP. Samsung and Qualcomm, along with some help from Powermat, SK Telecom, Ever Win Industries ..."),
                           QStringList(),
                           QDateTime::currentDateTime(),
                           QString("by Jon Fingas"),
                           false,
                           QUrl("http://www.engadget.com/2012/05/08/samsung-qualcomm-start-alliance-for-wireless-power/"),
                           QString("fakefeeder"),
                           QString("fakefeeder"));

    emit eventsFed();
}
