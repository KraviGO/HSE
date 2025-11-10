import React from "react";
import { LineChart, Line, XAxis, YAxis, Tooltip, ResponsiveContainer, Legend } from "recharts";

export default function BalanceOverTime({ data }) {
    return (
        <ResponsiveContainer width="100%" height={320}>
            <LineChart data={data}>
                <XAxis dataKey="date" />
                <YAxis />
                <Tooltip />
                <Legend />
                <Line type="monotone" dataKey="value" stroke="#42a5f5" dot={false} />
            </LineChart>
        </ResponsiveContainer>
    );
}