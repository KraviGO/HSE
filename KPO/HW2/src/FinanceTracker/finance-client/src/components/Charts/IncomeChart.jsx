import React from "react";
import { BarChart, Bar, XAxis, YAxis, Tooltip, Legend, ResponsiveContainer } from "recharts";

export default function IncomeChart({ data, color = "#66bb6a" }) {
    return (
        <ResponsiveContainer width="100%" height={320}>
            <BarChart data={data}>
                <XAxis dataKey="name" />
                <YAxis />
                <Tooltip />
                <Legend />
                <Bar dataKey="value" fill={color} />
            </BarChart>
        </ResponsiveContainer>
    );
}