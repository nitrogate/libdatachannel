/**
 * Copyright (c) 2020 Filip Klembara (in2core)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RTC_RTCP_SENDER_REPORTABLE_H
#define RTC_RTCP_SENDER_REPORTABLE_H

#if RTC_ENABLE_MEDIA

#include "message.hpp"
#include "rtppacketizationconfig.hpp"
#include "messagehandlerelement.hpp"

namespace rtc {

class RTC_CPP_EXPORT RtcpSRReporter: public MessageHandlerElement {

	bool needsToReport = false;

	uint32_t packetCount = 0;
	uint32_t payloadOctets = 0;
	double timeOffset = 0;

	uint32_t _previousReportedTimestamp = 0;

	void addToReport(RTP *rtp, uint32_t rtpSize);
	message_ptr getSenderReport(uint32_t timestamp);

public:
	static uint64_t secondsToNTP(double seconds);

	/// Timestamp of previous sender report
	const uint32_t &previousReportedTimestamp = _previousReportedTimestamp;

	/// RTP configuration
	const std::shared_ptr<RtpPacketizationConfig> rtpConfig;

	RtcpSRReporter(std::shared_ptr<RtpPacketizationConfig> rtpConfig);

	ChainedOutgoingProduct processOutgoingBinaryMessage(ChainedMessagesProduct messages, std::optional<message_ptr> control) override;

	/// Set `needsToReport` flag. Sender report will be sent before next RTP packet with same
	/// timestamp.
	void setNeedsToReport();

	/// Set offset to compute NTS for RTCP SR packets. Offset represents relation between real start
	/// time and timestamp of the stream in RTP packets
	/// @note `time_offset = rtpConfig->startTime_s -
	/// rtpConfig->timestampToSeconds(rtpConfig->timestamp)`
	void startRecording();
};

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */

#endif /* RTC_RTCP_SENDER_REPORTABLE_H */
