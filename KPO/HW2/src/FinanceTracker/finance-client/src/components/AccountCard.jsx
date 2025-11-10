import React from "react";
import { Card, CardContent, Typography } from "@mui/material";

export default function AccountCard({ name, currency, balance }) {
    const color = balance >= 0 ? "success.main" : "error.main";
    return (
        <Card variant="outlined" sx={{ height: "100%" }}>
            <CardContent>
                <Typography variant="h6">{name}</Typography>
                <Typography variant="body2" color="text.secondary">
                    {currency}
                </Typography>
                <Typography variant="h5" sx={{ mt: 1, color }}>
                    {balance.toFixed(2)} ₽
                </Typography>
            </CardContent>
        </Card>
    );
}